// ======================================================================
// pillbox_tm010_1p5D.pro
//
// TM010 eigenmode of an axisymmetric pillbox cavity (1.5D = 2D meridian + 2πr weight)
//
// IMPORTANT (geometry/mesh):
//   - This .pro expects a 2D meridian mesh (X = axial, Y = radius), NOT the full 3D rotated mesh.
//   - Physical tags expected in the .msh:
//       Surface(1)  : Vacuum domain (the 2D rectangle)
//       Curve(20)   : Axis (Y=0)
//       Curve(21)   : PEC wall (all conductor curves: z=0, r=R, z=L)
//       Curve(22)   : Side wall only (r=R)  <-- Dirichlet for Ez in this scalar TM010 model
//
// What it does:
//   1) Solve generalized eigenproblem for Ez (scalar) -> eigenvalue λ = ω^2
//   2) Post-process Ez, Hphi, Hr (=0 for m=0 TM) distributions
//   3) Compute stored energy U, wall loss Ploss (skin-depth model), Q0 and R/Q
//
// Notes:
//   - This scalar Ez formulation is appropriate for TM0n0-like modes (TM010 here).
//   - For modes with nontrivial Er (e.g., TM01p with p>0), a full vector formulation is safer.
// ======================================================================

DefineConstant[
  // Physical constants
  mu0 = 4.e-7 * Pi,
  eps0 = 8.854187817e-12,

  // Wall material conductivity (S/m). Copper ~ 5.8e7
  sigma_wall = 5.8e7,

  // Eigen solve control
  neig = 1,                 // only TM010
  f_target = 1.0e9,         // Hz (used only for spectral shift; set near expected f)
  shift_re = (2*Pi*f_target)^2,
  shift_im = 0.0,

  // FEM order (1 or 2 are typical)
  pOrder = 2
];

// -------------------------
// Groups (physical tags)
// -------------------------
Group{
  Vacuum  = Region[1];
  Axis    = Region[20];
  WallAll = Region[21];
  WallR   = Region[22];   // r = R only
}

// -------------------------
// Jacobian methods
// -------------------------
Jacobian{
  { Name Vol;
    Case{
      { Region All; Jacobian Vol; }
    }
  }
  { Name Sur;
    Case{
      { Region All; Jacobian Sur; }
    }
  }
}


// -------------------------
// Constraint (Dirichlet Ez=0 on side wall r=R)
// -------------------------
Constraint{
  { Name Ez0_on_WallR; Type Assign;
    Case{
      { Region WallR; Value 0.0; }
    }
  }
}

// -------------------------
// Integration rule
// -------------------------
Integration{
  { Name Int;
    Case{
      { Type Gauss;
        Case{
          { GeoElement Line;     NumberOfPoints 4; }
          { GeoElement Triangle; NumberOfPoints 7; }
          { GeoElement Quadrangle; NumberOfPoints 9; } // 四角形メッシュの時だけ必要（あっても害はほぼ無い）
        }
      }
    }
  }
}


// -------------------------
// Function space (scalar nodal Ez)
// -------------------------
FunctionSpace{
  { Name Hgrad_Ez; Type Form0;
    BasisFunction{
      { Name sn; NameOfCoef Ez; Function BF_Node;
        Support Vacuum; Entity NodesOf[All]; }
    }
    // polynomial order (if your GetDP build supports it)
    // NOTE: if your GetDP complains here, remove the line below and set mesh order in Gmsh.
    // PolynomialOrder { pOrder; }

    Constraint{
      { NameOfCoef Ez; EntityType NodesOf;
        NameOfConstraint Ez0_on_WallR; }
    }
  }
}

// -------------------------
// Formulation: axisymmetric scalar Helmholtz eigenproblem for Ez
//
// Weak form (with 1.5D weight 2πr = 2πY[]):
//   ∫ (2πr/mu0) ∇Ez·∇v dA  -  ω^2 ∫ (2πr*eps0) Ez v dA = 0
//
// Eigenvalue λ = ω^2  (rad^2/s^2)
// -------------------------
Formulation{
  { Name Helmholtz_Ez; Type FemEquation;
    Quantity{
      { Name Ez; Type Local; NameOfSpace Hgrad_Ez; }
    }
    Equation{
      // Stiffness term
      Galerkin{
        [ (2*Pi*Y[]/mu0) * Dof{d Ez} , {d Ez} ] ;
        In Vacuum; Jacobian Vol; Integration Int;
      }

      // Mass term multiplied by eigenvalue (λ = ω^2)
      Galerkin{
        DtDtDof[ -(2*Pi*Y[]*eps0) * Dof{Ez} , {Ez} ] ;
        //Order 1;
        In Vacuum; Jacobian Vol; Integration Int;
      }
    }
  }
}

// -------------------------
// Resolution: eigen solve (Arpack/SLEPc)
// -------------------------
Resolution{
  { Name Solve_TM010;
    System{
      { Name Sys_Ez; NameOfFormulation Helmholtz_Ez; Type Real; }
    }
    Operation{
      GenerateSeparate[Sys_Ez];
      // EigenSolve[system, nEig, shift_re, shift_im, <optional filter>]
      EigenSolve[Sys_Ez, neig, shift_re, shift_im, ($EigenvalueReal > 0)];
    }
  }
}

// -------------------------
// Post-processing: fields + global integrals
// -------------------------
PostProcessing{
  { Name PP_TM010; NameOfFormulation Helmholtz_Ez;
    Quantity{
      // --- Fields (local) ---
      { Name Ez_map;
        Value{
          Local{ [ {Ez} ]; In Vacuum; }
        }
      }

      // Hphi from curl(E):
      //   Hphi = -(1/(ω μ0)) * ∂Ez/∂r  with r = Y
      { Name Hphi_map;
        Value{
          Local{
            [ -CompY[{d Ez}] / (mu0 * Sqrt[$EigenvalueReal]) ];
            In Vacuum;
          }
        }
      }

      // For m=0 TM modes in this scalar Ez model, Hr is identically 0
      { Name Hr_map;
        Value{
          Local{ [ 0.0 ]; In Vacuum; }
        }
      }

      // --- Global quantities (integral values; global sum done in PostOperation) ---

      // Stored energy U = (1/4)∫(ε|E|^2 + μ|H|^2)dV
      // with dV = 2πr dA = 2πY[] dA
      { Name U_stored;
        Value{
          Integral{
            [ 2*Pi*Y[] * (
                eps0/4 * SquNorm[{Ez}] +
                mu0/4 * SquNorm[ -CompY[{d Ez}] / (mu0 * Sqrt[$EigenvalueReal]) ]
              )
            ];
            In Vacuum; Jacobian Vol; Integration Int;
          }
        }
      }

      // Accelerating voltage along axis (no transit-time factor):
      //   Vacc = ∫ Ez(r=0, z) dz  (line integral along Axis curve)
      { Name Vacc;
        Value{
          Integral{
            [ {Ez} ];
            In Axis; Jacobian Sur; Integration Int;
          }
        }
      }

      // Wall loss using classical surface resistance:
      //   Rs = sqrt( ω*mu0 / (2*sigma) )
      //   Ploss = (1/2) ∫ Rs |H_t|^2 dS  with dS = 2πr ds = 2πY[] ds
      { Name Ploss;
        Value{
          Integral{
            [ (2*Pi*Y[]) *
              (0.5 * Sqrt[ (Sqrt[$EigenvalueReal]*mu0) / (2*sigma_wall) ]) *
              SquNorm[ -CompY[{d Ez}] / (mu0 * Sqrt[$EigenvalueReal]) ]
            ];
            In WallAll; Jacobian Sur; Integration Int;
          }
        }
      }
    }
  }
}

// -------------------------
// Post-operation: export maps + compute Q0 and R/Q
// -------------------------
PostOperation{
  { Name PO_TM010;
    NameOfPostProcessing PP_TM010;
    Operation{
      // Field maps
      Print[ Ez_map,   OnElementsOf Vacuum, File "TM010_Ez.pos" ];
      Print[ Hphi_map, OnElementsOf Vacuum, File "TM010_Hphi.pos" ];
      Print[ Hr_map,   OnElementsOf Vacuum, File "TM010_Hr.pos" ];

      // Store integrals in run-time variables (StoreInVariable works with OnRegion)
      Print[ U_stored, OnRegion Vacuum, StoreInVariable $U ];
      Print[ Ploss,    OnRegion WallAll, StoreInVariable $Pl ];
      Print[ Vacc,     OnRegion Axis, StoreInVariable $V ];

      // Summary (single line table):
      // columns: f(Hz), U(J), Ploss(W), Vacc(V), Q0, R_over_Q(Ohm)
      Print[
        { Sqrt[$EigenvalueReal]/(2*Pi),
          $U,
          $Pl,
          $V,
          (Sqrt[$EigenvalueReal] * $U) / $Pl,
          ($V*$V) / (Sqrt[$EigenvalueReal] * $U)
        },
        Format "%.16g %.16g %.16g %.16g %.16g %.16g\n",
        File "TM010_summary.txt", Format Table
      ];
    }
  }
}
