// axi_cavity_eigen.pro
// Axisymmetric (r-z) cavity eigenmodes (TM, m=0) using curl-curl E formulation
// - EigenSolve + DtDtDof (generalized EVP)
// - Computes: f, Q0 (given Rs), R/Q, r/Q, geometry factor G

// -----------------------------
// Parameters (edit or override)
DefineConstant[
  FreqTarget = {1e9, Name "Target frequency [Hz]"},
  NumModes   = {5,   Name "Number of eigenmodes"},
  Rs         = {1e-3, Name "Surface resistance Rs [Ohm] (for Q0)"},
  beta       = {1.0, Name "Particle beta for R/Q (1.0=ultra-relativistic)"}
];

// -----------------------------
// Groups from mesh physical tags
Group{
  Omega = Region[1]; // 2D surface: cavity cross-section
  Wall  = Region[2]; // 1D lines: conductor boundary (no axis)
  Axis  = Region[3]; // 1D line: r=0 (for Vacc integral)
}

// -----------------------------
// Physical constants
Function{
  Pi   = 3.1415926535897932384626433832795;
  mu0  = 4e-7 * Pi;
  eps0 = 8.8541878128e-12;
  c0   = 1/Sqrt[mu0*eps0];
}

// -----------------------------
// PEC wall: tangential E = 0 on Wall
Constraint{
  { Name E_tangential;
    Case{
      { Region Wall; Value 0.0; }
    }
  }
}

// -----------------------------
// Minimal Jacobian & Integration
Jacobian{
  { Name JVol;  Case { { Region All; Jacobian Vol;  } } }
  { Name JSurf; Case { { Region All; Jacobian Surf; } } }
}

Integration{
  { Name I1;
    Case{
      { Type Gauss;
        Case{
          { GeoElement Triangle; NumberOfPoints 4; }
          { GeoElement Line;     NumberOfPoints 3; }
        }
      }
    }
  }
}

// -----------------------------
// H(curl) space for E in (r,z) plane (edge elements)
FunctionSpace{
  { Name HcurlE;
    Type Form1;
    BasisFunction{
      { Name se; NameOfCoef ee; Function BF_Edge;
        Support Omega;
        Entity EdgesOf[Omega];
      }
    }
    Constraint{
      { NameOfCoef ee;
        EntityType EdgesOf;
        NameOfConstraint E_tangential;
      }
    }
  }
}

// -----------------------------
// Maxwell eigenproblem in vacuum (PEC boundary):
// curl curl E = ω^2 μ0 ε0 E
//
// Axisymmetric reduction for m=0 with E=(Er,Ez) has curlE only in φ direction,
// but we can keep a generic Curl operator; the 3D weighting is 2πr (=2π*$X).
Formulation{
  { Name MaxwellEigen_E;
    Type FemEquation;
    Quantity{
      { Name e; Type Local; NameOfSpace HcurlE; }
    }
    Equation{
      // K: ∫ (1/μ0) |curl E|^2 dV  ->  2π ∫ (1/μ0) |curl E|^2 r dA
      Integral{
        [ (2*Pi*$X/mu0) * Dof{Curl e},
          {Curl e}
        ];
        In Omega;
        Jacobian JVol;
        Integration I1;
      }

      // M: ∫ ε0 |E|^2 dV  ->  2π ∫ ε0 |E|^2 r dA
      // Eigenvalue is ω^2: K - ω^2 M = 0 (DtDtDof term supplies the ω^2 factor)
      Integral{
        DtDtDof[
          -(2*Pi*$X*eps0) * Dof{e},
          {e}
        ];
        In Omega;
        Jacobian JVol;
        Integration I1;
      }
    }
  }
}

// -----------------------------
// Solve generalized EVP near a target frequency
Resolution{
  { Name CavityEigen;
    System{
      { Name Sys;
        NameOfFormulation MaxwellEigen_E;
        Type Real;
      }
    }
    Operation{
      GenerateSeparate[Sys];
      EigenSolve[Sys, NumModes, (2*Pi*FreqTarget)^2, 0, ($EigenvalueReal > 0)];
      SaveSolutions[Sys];
    }
  }
}

// -----------------------------
// PostProcessing: integrals for Q0 and R/Q
PostProcessing{
  { Name CavityPP;
    NameOfFormulation MaxwellEigen_E;
    Quantity{
      // fields for visualization
      { Name E; Value{ Local{ [ {e} ]; In Omega; Jacobian JVol; } } }
      { Name Hphi; Value{
          // TM(m=0): Hφ = |curlE|/(μ0 ω). (phase factor j omitted; magnitude is what Q needs)
          Local{ [ {Curl e}/(mu0*Sqrt[$EigenvalueReal]) ]; In Omega; Jacobian JVol; }
        }
      }

      // Stored energies (time-average)
      { Name Ue; Value{
          Integral{ [ 0.25*eps0*SquNorm[{e}] * (2*Pi*$X) ];
                    In Omega; Jacobian JVol; Integration I1; }
        }
      }
      { Name Um; Value{
          // Um = (1/(4 μ0 ω^2)) ∫ |curlE|^2 dV
          Integral{ [ 0.25*(1/(mu0*$EigenvalueReal))*SquNorm[{Curl e}] * (2*Pi*$X) ];
                    In Omega; Jacobian JVol; Integration I1; }
        }
      }

      // Wall loss with surface resistance Rs:
      // Pwall = (Rs/2) ∫ |H_t|^2 dS ; for TM0, H_t = Hφ
      { Name Pwall; Value{
          Integral{ [ 0.5*Rs*(1/(mu0*mu0*$EigenvalueReal))*SquNorm[{Curl e}] * (2*Pi*$X) ];
                    In Wall; Jacobian JSurf; Integration I1; }
        }
      }

      // Accelerating voltage Vacc(β): ∫ Ez exp(-j ω z/(βc)) dz along axis
      { Name Vacc; Value{
          Integral{ [ CompY[{e}] * Exp[ Complex[0, -Sqrt[$EigenvalueReal]*$Y/(beta*c0)] ] ];
                    In Axis; Jacobian JSurf; Integration I1; }
        }
      }

      { Name Laxis; Value{
          Integral{ [ 1.0 ]; In Axis; Jacobian JSurf; Integration I1; }
        }
      }
    }
  }
}

// -----------------------------
// PostOperation: write cavity parameters and fields
PostOperation{
  { Name CavityParams;
    NameOfPostProcessing CavityPP;
    Operation{
      // CSV columns:
      // modeIndex, f[Hz], Q0, R_over_Q[Ohm], r_over_Q[Ohm/m], G[Ohm], Rsh[Ohm]
      Print[
        { $TimeStep,
          Sqrt[$EigenvalueReal]/(2*Pi),

          // Q0 = ω U / Pwall
          (Sqrt[$EigenvalueReal]*(Ue[Omega] + Um[Omega])) / Pwall[Wall],

          // R/Q = |Vacc|^2 / (ω U)
          SquNorm[ Vacc[Axis] ] / (Sqrt[$EigenvalueReal]*(Ue[Omega] + Um[Omega])),

          // r/Q = (R/Q) / Laxis
          (SquNorm[ Vacc[Axis] ] / (Sqrt[$EigenvalueReal]*(Ue[Omega] + Um[Omega]))) / Laxis[Axis],

          // Geometry factor G = Q0 * Rs
          ((Sqrt[$EigenvalueReal]*(Ue[Omega] + Um[Omega])) / Pwall[Wall]) * Rs,

          // Shunt impedance Rsh = (R/Q) * Q0
          (SquNorm[ Vacc[Axis] ] / (Sqrt[$EigenvalueReal]*(Ue[Omega] + Um[Omega]))) *
          ((Sqrt[$EigenvalueReal]*(Ue[Omega] + Um[Omega])) / Pwall[Wall])
        },
        File "cavity_params.csv",
        Format "%g,%g,%g,%g,%g,%g,%g\n"
      ];
    }
  }

  { Name Fields;
    NameOfPostProcessing CavityPP;
    Operation{
      Print[ E,    OnElementsOf Omega, File "E.pos"    ];
      Print[ Hphi, OnElementsOf Omega, File "Hphi.pos" ];
    }
  }
}
