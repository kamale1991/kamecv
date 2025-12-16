// pillbox_tm010_axi.pro
// GetDP input for 1.5D (axisymmetric) eigenmode calculation of TM010-like modes
// using a scalar unknown hphi(r,z) = H_phi (azimuthal magnetic field component).
//
// This solves the axisymmetric Maxwell eigenproblem (m=0):
//   curl( (1/eps0) curl H ) = omega^2 * mu0 * H
// with H = hphi * e_phi.
//
// Weak form (axisymmetric volume element 2*pi*r dr dz is handled by VolAxi):
//   ∫ (1/eps0) [ (∂h/∂z)(∂v/∂z) + (∂h/∂r + h/r)(∂v/∂r + v/r) ] dV
//   - omega^2 ∫ mu0 h v dV = 0
//
// Boundary conditions for a PEC cavity are natural (Neumann) for hphi; we only enforce
// regularity on the symmetry axis: hphi(r=0,z)=0.
//
// Post-processing computes:
//   - f = omega/(2*pi)
//   - stored energy U = (mu0/2) ∫ |H|^2 dV
//   - ohmic wall loss P = (Rs/2) ∮ |H_t|^2 dS  (Rs from wall conductivity)
//   - Q0 = omega*U/P
//   - shunt impedance Rsh = V^2/P, and R/Q = V^2/(omega*U)
//
// IMPORTANT:
/// 1) This assumes a *closed* cavity with perfect electric conductor boundaries.
//    No external-Q, no couplers, no beam pipes.
// 2) "Shunt impedance" conventions differ (peak vs RMS, with/without transit time factor).
//    This file reports both:
//       - Vdc: plain line integral ∫ Ez dz  (no transit-time factor)
//       - Vttf: |∫ Ez exp(i*k0*z) dz| for beta=1 (includes transit time factor)

DefineConstant[
  fShift = {5.712e9, Name "00_Input/Target eigenfrequency for shift [Hz]"},
  NbEig  = {1,       Name "00_Input/Number of eigenpairs"},
  sigma  = {5.8e7,   Name "00_Input/Wall conductivity [S/m]"},
  c0     = {299792458, Name "00_Input/Speed of light [m/s]"}
];

// --------------------------------------------------------------------------
// Geometry regions (from Gmsh Physical tags)
Group{
  Omega = Region[101];      // cavity vacuum cross-section (2D surface)
  Walls = Region[201];      // PEC walls (1D lines, revolved -> 2D surface)
  Axis  = Region[202];      // symmetry axis r=0 (1D line)
}

// --------------------------------------------------------------------------
// Constants
Function{
  mu0  = 4e-7*Pi;
  eps0 = 1/(mu0*c0*c0);

  // Shift around omega^2
  omegaShift  = 2*Pi*fShift;
  lambdaShift = omegaShift*omegaShift;
}

// --------------------------------------------------------------------------
// Jacobians and integration
Jacobian{
  { Name JVol;
    Case{
      { Region Omega; Jacobian VolAxi; } // axisymmetric volume (includes 2*pi*r)
    }
  }
  { Name JWall;
    Case{
      { Region Walls; Jacobian SurAxi; } // axisymmetric surface (includes 2*pi*r)
    }
  }
  { Name JAxis;
    Case{
      { Region Axis; Jacobian Sur; }     // plain line integral along the axis (NO 2*pi*r!)
    }
  }
}

Integration{
  { Name Int;
    Case{
      { Type Gauss;
        Case{
          { GeoElement Triangle;   NumberOfPoints 4; }
          { GeoElement Line;       NumberOfPoints 4; }
        }
      }
    }
  }
}

// --------------------------------------------------------------------------
// Constraints: regularity on axis (hphi=0 at r=0)
Constraint{
  { Name HphiAxis0;
    Case{
      { Region Axis; Type Assign; Value 0; }
    }
  }
}

// --------------------------------------------------------------------------
// Function space: scalar nodal approximation for hphi(r,z)
FunctionSpace{
  { Name Hphi; Type Form0;
    BasisFunction{
      { Name sn; NameOfCoef hn; Function BF_Node;
        Support Omega; Entity NodesOf[All]; }
    }
    Constraint{
      { NameOfCoef hn; EntityType NodesOf; NameOfConstraint HphiAxis0; }
    }
  }
}

// --------------------------------------------------------------------------
// Formulation: eigenvalue problem for hphi
Formulation{
  { Name TM_hphi_Eigen; Type FemEquation;
    Quantity{
      { Name h; Type Local; NameOfSpace Hphi; }
    }
    Equation{

        // r-operator term: (∂h/∂r + h/r)(∂v/∂r + v/r)
        // Expand into 4 Galerkin blocks to avoid mixing Dof{d h} and Dof{h}

        // (∂h/∂r)(∂v/∂r)
        Galerkin{
        [ (1/eps0) * CompX[Dof{d h}], CompX[{d h}] ];
        In Omega; Jacobian JVol; Integration Int;
        }

        // (h/r)(∂v/∂r)
        Galerkin{
        [ (1/eps0) * (1/X[]) * Dof{h}, CompX[{d h}] ];
        In Omega; Jacobian JVol; Integration Int;
        }

        // (∂h/∂r)(v/r)
        Galerkin{
        [ (1/eps0) * (1/X[]) * CompX[Dof{d h}], {h} ];
        In Omega; Jacobian JVol; Integration Int;
        }

        // (h/r)(v/r)
        Galerkin{
        [ (1/eps0) * (1/(X[]*X[])) * Dof{h}, {h} ];
        In Omega; Jacobian JVol; Integration Int;
        }

      // Mass term: -omega^2 * mu0 * h v
      Galerkin{
        DtDtDof[ -mu0 * Dof{h}, {h} ];
        In Omega; Jacobian JVol; Integration Int;
      }
    }
  }
}

// --------------------------------------------------------------------------
// Post-processing
PostProcessing{
  { Name TM_hphi_PP; NameOfFormulation TM_hphi_Eigen;
    Quantity{
      // omega = sqrt(eigenvalue), f = omega/(2*pi)
      { Name omega; Value{ Local{ [ Sqrt[$EigenvalueReal] ]; In Omega; } } }
      { Name freq;  Value{ Local{ [ Sqrt[$EigenvalueReal]/(2*Pi) ]; In Omega; } } }

      // Surface resistance Rs = sqrt(omega*mu0/(2*sigma))
      { Name Rsurf; Value{ Local{ [ Sqrt[Sqrt[$EigenvalueReal]*mu0/(2*sigma)] ]; In Omega; } } }
      // Stored energy (total) U = (mu0/2) ∫ |H|^2 dV; here H=Hphi=h
      { Name U; Value{
          Integral{ [ 0.5*mu0*{h}*{h} ]; In Omega; Jacobian JVol; Integration Int; }
      } }

      // Ohmic wall loss P = (Rs/2) ∮ |H_t|^2 dS; here H_t=Hphi=h on walls
      { Name Ploss; Value{
          Integral{ [ 0.5*Sqrt[Sqrt[$EigenvalueReal]*mu0/(2*sigma)]*{h}*{h} ];
                    In Walls; Jacobian JWall; Integration Int; }
      } }

      // Axis field for accelerating voltage.
      // Use the r->0 limit: (1/r)∂(r h)/∂r -> 2*∂h/∂r at r=0,
      // so Ez_axis = 2/(omega*eps0)*∂h/∂r.
      { Name Vdc; Value{
          Integral{ [ (2*CompX[{d h}]) / (Sqrt[$EigenvalueReal]*eps0) ];
                    In Axis; Jacobian JAxis; Integration Int; }
      } }

      // Transit-time-factor voltage for beta=1:
      // Vttf = | ∫ Ez(z) * exp(i*k0*z) dz |, with k0 = omega/c0 and z = Y[]
      { Name Vcos; Value{
          Integral{ [ ((2*CompX[{d h}]) / (Sqrt[$EigenvalueReal]*eps0)) *
                      Cos[(Sqrt[$EigenvalueReal]/c0)*Y[]] ];
                    In Axis; Jacobian JAxis; Integration Int; }
      } }
      { Name Vsin; Value{
          Integral{ [ ((2*CompX[{d h}]) / (Sqrt[$EigenvalueReal]*eps0)) *
                      Sin[(Sqrt[$EigenvalueReal]/c0)*Y[]] ];
                    In Axis; Jacobian JAxis; Integration Int; }
      } }

      // Derived RF figures
      /*
      { Name Q0; Value{
          Local{ [ Sqrt[$EigenvalueReal] * U[Omega] / Ploss[Walls] ]; In Omega; }
      } }

      { Name R_over_Q_dc; Value{
          Local{ [ (Vdc[Axis]^2)  / (Sqrt[$EigenvalueReal] * U[Omega]) ]; In Omega; }
      } }
      { Name Rsh_dc; Value{
          Local{ [ (Vdc[Axis]^2)  / Ploss[Walls] ]; In Omega; }
      } }
      { Name R_over_Q_ttf; Value{
          Local{ [ (Vttf[Omega]^2) / (Sqrt[$EigenvalueReal] * U[Omega]) ]; In Omega; }
      } }
      { Name Rsh_ttf; Value{
          Local{ [ (Vttf[Omega]^2) / Ploss[Walls] ]; In Omega; }
      } }
        */

      // Optional: field output for visualization (Hphi on Omega)
      { Name HphiField; Value{ Local{ [ {h} ]; In Omega; } } }
    }
  }
}

PostOperation{
  { Name TM_hphi_PO; NameOfPostProcessing TM_hphi_PP;
    Operation{
      //Print[ freq[Omega],         OnGlobal, File "tm010_results.txt", Format Table ];
      //Print[ omega[Omega],        OnGlobal, File "tm010_results.txt", Format Table ];
      //Print[ Rsurf[Omega],        OnGlobal, File "tm010_results.txt", Format Table ];

      //Print[ U,             OnRegion Omega, File "tm010_results.txt", Format Table ];
      //Print[ Ploss,         OnRegion Omega, File "tm010_results.txt", Format Table ];

      //Print[ Vdc[Axis],            OnGlobal, File "tm010_results.txt", Format Table ];

      // --- まずグローバル積分値を計算してレジスタへ ---
      Print[ U,     OnRegion Omega, StoreInRegister 21, NoNewLine ];
      Print[ Ploss, OnRegion Walls, StoreInRegister 22, NoNewLine ];

      // Vcos, Vsin は「Integral{...}」で定義した post-quantity であることが前提
      // まず総和した値をレジスタに格納
      Print[ Vcos, OnRegion Axis, StoreInRegister 11, NoNewLine ];
      Print[ Vsin, OnRegion Axis, StoreInRegister 12, NoNewLine ];
      // Vttf を計算してレジスタ13に格納しつつ出力
      Print[
        { ( Sqrt[ #11^2 + #12^2 ] )#13 },
        Format "Vttf = %.16g\n",
        File > "tm010_results.txt"
      ];      



      // --- 固有値の符号は定義次第でマイナスになり得るので Abs を噛ませる ---
      // Q0 = omega * U / P
      Print[ { ( Sqrt[Abs[$EigenvalueReal]] * #21 / #22 )#23 },
             Format "Q0 = %.16g\n",
             File >> "tm010_results.txt" ];          
             
      Print[ { (#13^2 / (Sqrt[Abs[$EigenvalueReal]] * #21))#31 },
             Format "RoverQ_ttf = %.16g\n",
             File >> "tm010_results.txt" ];

      Print[ { (#13^2 / #22)#32 },
             Format "Rsh_ttf   = %.16g\n",
             File >> "tm010_results.txt" ];             

      //Print[ Vttf[Omega],         OnGlobal, File "tm010_results.txt", Format Table ];

      //Print[ Q0[Omega],           OnGlobal, File "tm010_results.txt", Format Table ];
      //Print[ R_over_Q_dc[Omega],  OnGlobal, File "tm010_results.txt", Format Table ];
      //Print[ Rsh_dc[Omega],       OnGlobal, File "tm010_results.txt", Format Table ];
      //Print[ R_over_Q_ttf[Omega], OnGlobal, File "tm010_results.txt", Format Table ];
      //Print[ Rsh_ttf[Omega],      OnGlobal, File "tm010_results.txt", Format Table ];

      // Field plot (optional)
      Print[ HphiField, OnElementsOf Omega, File "tm010_hphi.pos" ];
    }
  }
}

// --------------------------------------------------------------------------
// Resolution: eigenmode solve around lambdaShift (= omegaShift^2)
Resolution{
  { Name TM_hphi_Res;
    System{
      { Name Sys; NameOfFormulation TM_hphi_Eigen; }
    }
    Operation{
      GenerateSeparate[Sys];

      // Solve for eigenpairs near the shift.
      // Syntax: EigenSolve[system, number, shiftReal, shiftImag <, filter>]
      EigenSolve[Sys, NbEig, lambdaShift, 0];

      SaveSolutions[Sys];
      PostOperation[TM_hphi_PO];
    }
  }
}
