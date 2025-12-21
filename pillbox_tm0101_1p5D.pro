// ======================================================================
// pillbox_tm010_1p5D.pro
//
// The eigenmode of an axisymmetric pillbox cavity (1.5D = 2D meridian + 2πr weight)
//
// IMPORTANT (geometry/mesh):
//   - This .pro expects a 2D meridian mesh (Y = axial, X = radius), NOT the full 3D rotated mesh.
//   - Physical tags expected in the .msh:
//       Surface(101)  : Vacuum domain (the 2D rectangle)
//       Line(202)   : Axis (X=0)
//       Line(201)   : PEC wall (all conductor curves: z=0, r=R, z=L)
//
// What it does:
//   1) Solve generalized eigenproblem for E (1-Form) (m=0)-> eigenvalue λ = ω^2
//   2) Post-process frequency, E, H
//   3) Compute stored energy U, wall loss Ploss (skin-depth model), Q0 and R/Q
//
// Notes:
//   - This is a simplified 2.5D (axisymmetric) model for the m=0 modes only.
// ======================================================================


DefineConstant[
  // Physical constants
  f0  = 5.712e9,             // target frequency [Hz] (reference)
  c0  = 299792458,           // speed of light [m/s]
  x01 = 2.404825557695773,   // J0 first root

  R = x01 * c0 / (2*Pi*f0),  // ~ 20.088 mm for 5.712 GHz
  L = c0/(2*f0),             // example length: lambda/2 ~ 26.24 mm (edit freely)
  mu0 = 4.e-7 * Pi,
  eps0 = 1.0 / (mu0 * c0 * c0),

  // Wall material conductivity (S/m). Copper ~ 5.8e7
  sigma_wall = 5.8e7,

  // Eigen solve control
  neig = 1,                 // only TM010
  f_target = f0,         // Hz (used only for spectral shift; set near expected f)
  shift_re = (2*Pi*f_target)^2,
  shift_im = 1.0,

  // FEM order (1 or 2 are typical)
  pOrder = 2  
];

Function {
  eps[] = eps0;
  mu[]  = mu0;
  w[] = 2.0*Pi*Y[];
}

// -------------------------
// Groups (physical tags)
// -------------------------
Group{
  Omega  = Region[101];  
  AllWall = Region[201];
  Axis    = Region[202];  
}

// -------------------------
// Jacobian methods
// -------------------------
Jacobian{
  { Name JVol;
    Case{
      { Region Omega; Jacobian VolAxi; } // 2.5 D
    }
  }
  { Name JWall;
    Case{
      { Region AllWall; Jacobian SurAxi; } // 2.5 D 
    }
  }
  { Name JAxis;
    Case{
      { Region Axis; Jacobian Sur; }
    }
  }
}


// -------------------------
// Constraint (Dirichlet Ez=0 on side wall r=R)
// -------------------------
Constraint{
  { Name PEC; Type Assign;
    Case{
      { Region AllWall; Value 0.0; }
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
          { GeoElement Line;       NumberOfPoints 4; }
          { GeoElement Triangle;   NumberOfPoints 4; }
          { GeoElement Quadrangle; NumberOfPoints 4; } // 四角形メッシュの時だけ必要（あっても害はほぼ無い）
        }
      }
    }
  }
}

// -------------------------
// Function space
// -------------------------
FunctionSpace{
  { Name Hcurl; Type Form1;
    BasisFunction{
      { Name sn; NameOfCoef en; Function BF_Edge;
        Support Region[{Omega, Axis, AllWall}]; Entity EdgesOf[All]; }
    }

    Constraint{
      { NameOfCoef en; EntityType EdgesOf;
        NameOfConstraint PEC; }
    }
  }
}

// -------------------------
// Formulation: axisymmetric Helmholtz for curl E (m = 0)
// -------------------------
Formulation{
  { Name Helmholtz_m0; Type FemEquation;
    Quantity{
      { Name E; Type Local; NameOfSpace Hcurl; }
      { Name Ploss2; Type Integral; [CompY[{E} ]] ; In Axis; Jacobian JAxis; Integration Int;  }
    }
    Equation{
      // Stiffness term
      Galerkin{
        [ ( 1.0 / mu[] ) * Dof{Curl E} , {Curl E} ] ;
        In Omega; Jacobian JVol; Integration Int;
      }

      // Mass term multiplied by eigenvalue (λ = ω^2)
      Galerkin{
        DtDtDof[ eps[] * Dof{E} , {E} ] ;
        //Order 1;
        In Omega; Jacobian JVol; Integration Int;
      }
    }
  }
}

// -------------------------
// Resolution: eigen solve (Arpack/SLEPc)
// -------------------------
Resolution{
  { Name Solve_m0;
    System{
      { Name Sys_E_m0;
        NameOfFormulation Helmholtz_m0; 
        Type Real;        
      }
    }
    Operation{
      GenerateSeparate[Sys_E_m0];      
      EigenSolve[Sys_E_m0, neig, shift_re, shift_im, ($EigenvalueReal > 0)];
      SaveSolutions[Sys_E_m0];
    }
  }
}

// -------------------------
// Post-processing: fields + global integrals
// -------------------------
PostProcessing{
  { Name PP_m0; NameOfFormulation Helmholtz_m0;
    Quantity{
      // --- EigenValue ---
      { Name freq; 
        Value{ 
            Local{
              [$EigenvalueReal / (2.0 * Pi) * 1e-9]; // in GHz
              In Omega;
            }
          }
      }

      // --- Fields (local) ---
      { Name E_map;
        Value{
          Local{ [ {E} ]; In Omega; Jacobian JVol; }
        }
      }

      { Name Ez_map;
        Value{
          Local{ [ CompY[ {E} ] ]; In Axis; Jacobian JAxis;}
        }
      }
             

      // Hphi from curl(E):      
      { Name H_map;
        Value{
          Local{
            [ - {Curl E} / (mu[] * $EigenvalueReal) ];
            In Omega;
            Jacobian JVol;
          }
        }
      }

      // --- Global quantities (integral values; global sum done in PostOperation) ---
      // Stored energy U = (1/4)∫(ε|E|^2 + μ|H|^2)dV
      // with dV = 2πr dA = 2πY[] dA
      { Name U_stored;
        Value{
          Integral{
            [ (
                eps[] / 4 * SquNorm[{E}] +
                mu[] / 4 * SquNorm[ -{Curl E} / (mu[] * $EigenvalueReal) ]
              )
            ];
            In Omega; Jacobian JVol; Integration Int;
          }
        }
      }

      // Accelerating voltage along axis (no transit-time factor):
      //   Vacc = ∫ Ez(r=0, z) dz  (line integral along Axis curve)
      
      { Name Vacc;
        Value{
          Integral{
            [ CompY[ {E} ] ];  // Ez component            
            In Axis; Jacobian JAxis; Integration Int;
          }
        }
      }
      
      { Name Vacc2;
        Value{
          Local { [ { Ploss2 } ] ; In Axis; Jacobian JAxis; }
        }
      }

      // Wall loss using classical surface resistance:
      //   Rs = sqrt( ω*mu0 / (2*sigma) )
      //   Ploss = (1/2) ∫ Rs |H_t|^2 dS  with dS = 2πr ds = 2πY[] ds
      { Name Ploss;
        Value{
          Integral{
            [
              (0.5 * Sqrt[ ($EigenvalueReal * mu[]) / (2 * sigma_wall) ]) *
              SquNorm[ -{Curl E} / (mu[] * $EigenvalueReal) ]              
            ];
            In AllWall; Jacobian JWall; Integration Int;
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
  { Name PO_m0;
    NameOfPostProcessing PP_m0;
    Operation{
      //Print[ freq, OnRegion Omega];
      Print[
        { $EigenvalueReal / (2*Pi) * 1e-9 },
        Format "f(GHz)=%.16g\n"
      ];
      // Field maps
      Print[ E_map, OnElementsOf Omega, File "pillbox_m0_E.pos" ];
      Print[ Ez_map, OnElementsOf Axis, File "pillbox_m0_Ez.pos" ];
      Print[ H_map, OnElementsOf Omega, File "pillbox_m0_Hphi.pos" ];     
      
      //Print[ Ez_map,
      // OnLine { {0, 0, 0} {0, L, 0} } {200},
      // File "pillbox_m0_Ez_axis.pos" 
      //];         

      // Store integrals in run-time variables (StoreInVariable works with OnRegion)
      Print[ U_stored[Omega], OnRegion Omega, StoreInVariable $U ];
      Print[ Ploss[AllWall],  OnRegion AllWall, StoreInVariable $Pl ];
      
      // Print[ Vacc[Axis],      OnRegion Axis, StoreInVariable $V];
      Print[ Vacc2[Axis],     OnGrid Axis, StoreInVariable $V];

      // Summary (single line table):
      // columns: f(Hz), U(J), Ploss(W), Vacc(V), Q0, R_over_Q(Ohm)
      
      Print[
        { $EigenvalueReal / (2*Pi) * 1e-9,
          $U,
          $Pl,
          $V,
          ($EigenvalueReal * $U) / $Pl,
          ($V*$V) / ($EigenvalueReal * $U)
        },
        Format "f=%.3f U=%.3f Ploss=%.3f Vacc=%.3f U=%.3f (R/Q)=%.3f\n",
        File "pillbox_m0_summary.txt", Format Table
      ];
      
    }
  }
}
// ======================================================================
