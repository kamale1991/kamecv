// ============================================================================
// pillbox_axisymmetric.cpp
//
// Mesh assumptions (2D meridian mesh: x=r, y=z):
//   Surface(101) : Vacuum domain (2D)
//   Line(201)    : PEC wall (conductor boundary)
//   Line(202)    : Axis (r=0)
//
// What this code does:
//   1) Solve generalized eigenproblem:  ∫ curl E · curl v dV = λ ∫ με E · v dV
//      (λ is ω^2)
//   2) Post-process: f, E, Hphi, U, Ploss, Q0, R/Q, Vacc
//   3) Export fields to Gmsh views
// ============================================================================

#include <gmshfem/GmshFem.h>
#include <gmshfem/Function.h>
#include <gmshfem/Formulation.h>
#include <gmshfem/Post.h>     // post::save, (and often post::integrate / post::integral)
#include <gmsh.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace gmshfem;
using namespace gmshfem::algebra;
using namespace gmshfem::common;
using namespace gmshfem::problem;
using namespace gmshfem::domain;
using namespace gmshfem::field;
using namespace gmshfem::function;
using namespace gmshfem::post;
using namespace gmshfem::equation;

using C = std::complex<double>;

void Projection(const Function<C, Degree::Degree1> &Func_in,
                field::Field<C, form::Form1> &Field_out,
                const std::complex<double> &weight,
                const Domain &dom,
                const std::string &gVol,
                const std::vector<Domain> &applyDirichiletWall){
  problem::Formulation<C> proj("L2proj");

  if (applyDirichiletWall.size() > 0) {
    // PEC wall: tangential E = 0
    for (const auto &wall : applyDirichiletWall)
      Field_out.addConstraint(wall, vector< C >(0., 0., 0.));  
  }

  proj.integral(
    weight * dof(Field_out),
    tf(Field_out),
    dom, gVol
  );

  proj.integral(
    weight * Func_in,
    tf(Field_out),
    dom, gVol
  );

  proj.pre();
  proj.assemble();
  proj.solve();

};

int main(int argc, char **argv)
{
  common::GmshFem fem(argc, argv);

  //const std::string mshFile = (argc > 1) ? argv[1] : "pillbox_tm010_1p5D.msh";
  const std::string mshFile = "pillbox_tm010_1p5D.msh";
  gmsh::open(mshFile);

  // -----------------------------
  // Constants (same as .pro)
  // -----------------------------
  const double f0  = 5.712e9;             // [Hz]
  const double c0  = 299792458.0;         // [m/s]
  const double x01 = 2.404825557695773;   // J0 first root

  const double R = x01 * c0 / (2.0 * M_PI * f0);
  const double L = c0 / (2.0 * f0);

  const double mu0  = 4e-7 * M_PI;
  const double eps0 = 1.0 / (mu0 * c0 * c0);

  const double sigma_wall = 5.8e7;  // copper ~ 5.8e7 S/m

  const int pOrder = 2;
  const int neig   = 3;

  // Target for shift-and-invert (typical generalized EVP uses λ = ω^2)
  const double f_target      = f0;
  const double lambda_target = std::pow(2.0 * M_PI * f_target, 2.0);  

  // -----------------------------
  // Domains (physical groups)
  // -----------------------------
  Domain Omega(2, 101); // 2D surface
  Domain Wall(1, 201);  // 1D boundary
  Domain Axis(1, 202);  // 1D boundary

  // -----------------------------
  // Axisymmetric weight
  // -----------------------------
  // Mesh convention: x = r, y = z
  // For "rotate meridian around z-axis": dV = 2π r dA, dS = 2π r ds
  auto r    = function::x<double>();
  auto wAxi = 2.0 * M_PI * r;  

  // EVP用
  auto rC = function::x<C>();
  auto wAxiC = 2.0 * M_PI * rC;

  // -----------------------------
  // Unknown field: E (H(curl), Form1)
  // -----------------------------
  Domain domE = Omega | Wall | Axis;

  //   functionSpaceHcurl::HierarchicalHcurl
  field::Field<std::complex<double>, form::Form1> E(
    "E", domE,
    functionSpaceHCurl::HierarchicalHCurl, // <-- adjust if your enum name differs
    pOrder
  );

  // PEC wall: tangential E = 0
  // (For Form1, addConstraint usually constrains the appropriate edge DoFs on the boundary.)
  E.addConstraint(Wall, vector< C >(0., 0., 0.));

  // -----------------------------
  // 1) Eigenproblem (Maxwell cavity)
  // -----------------------------
  problem::Formulation< C > evp("pillbox_evp");  

  const std::string gVol = "Gauss4"; // match .pro intent (4-point)
  const std::string gLin = "Gauss4";

  // Stiffness: ∫ curl(E)·curl(v) dV
  // Mass:      ∫ με E·v dV
  // --- Stiffness matrix term
  evp.integral(wAxiC * curl(dof(E)), curl(tf(E)), Omega, gVol);
  
  // --- Mass matrix term  
  evp.integral(wAxiC * C(eps0 * mu0) * dt2_dof(E), tf(E), Omega, gVol);
  // evp.setAngularFrequency(2.0 * M_PI * f_target); // for shift-and-invert
  evp.removeSystem();
  evp.initSystem();
  evp.pre();
  evp.assemble(true);

  // Solve eigenproblem:
  // Many setups pass SLEPc/PETSc options through argv, e.g.
  //   -eps_nev 1 -st_type sinvert -eps_target <lambda_target> -st_shift <lambda_target>
  // If your GmshFEM build exposes a direct API for target/shift, set it here.
  Vector< std::complex< double > > lambda;
  evp.eigensolve(lambda, true, neig, C(lambda_target));

  if(lambda.size() == 0) {
    std::cerr << "No eigenvalue returned.\n";
    return 1;
  }

    // -----------------------------
    // 2) Post-processing (fields + integrals)
    // -----------------------------
    // For m=0 TM-like modes on meridian mesh:
    //   Hphi = -(curl E)/(μ ω)  (sign/j is convention; losses use |H|^2 anyway)
    // auto curlE = curl(E);    

  {
    std::cout << "Check Eigen frequency [GHz]:" << std::endl;
    for (size_t i = 0; i < lambda.size(); ++i) {
      const double omega = std::real(std::sqrt(lambda[i]));
      const double fGHz_i = omega / (2.0*M_PI) / 1e9;
      std::cout << "  Mode " << i << ": f(GHz)=" << fGHz_i << "\n";

      //const auto E_i = eigenfunction(E, i);
      const auto Func_E_i = eigenfunction(E, i);    

      // -----------------------------
      // (A) L2 projection: Function (E_i) -> Field (Ei_h)
      // -----------------------------

      // 射影先の Field（E と同じ空間・同じ拘束を課すのが基本）
      field::Field<C, form::Form1> Ei_h(
        "E_mode_" + std::to_string(i),
        domE,
        functionSpaceHCurl::HierarchicalHCurl, // ←あなたの環境の enum 名に合わせる
        pOrder
      );

      std::vector<Domain> dirichiletWalls = {Wall};

      Projection(
        Func_E_i,
        Ei_h,
        wAxiC,
        Omega,
        gVol,
        dirichiletWalls  // PEC wall: tangential E = 0
      );

      /*

      {
        // PEC wall: tangential E = 0 （元の E と同じ拘束を付ける）
        Ei_h.addConstraint(Wall, vector<C>(0., 0., 0.));

        // 射影問題： M * Ei_h = b
        problem::Formulation<C> proj("L2proj_E_mode_" + std::to_string(i));      

        // 左辺（質量行列）: ∫ w * Ei_h · v dΩ
        // ここで dof(Ei_h) が trial、tf(Ei_h) が test
        proj.integral(
          wAxiC * dof(Ei_h),
          tf(Ei_h),
          Omega, gVol
        );

        // 右辺（ロード）: ∫ w * E_i · v dΩ
        // E_i は Function（eigenfunction から取ったもの）
        proj.integral(
          wAxiC * Func_E_i,
          tf(Ei_h),
          Omega, gVol
        );

        // 線形 solve（スライドの usage と同じ流れ）
        proj.pre();
        proj.assemble();
        proj.solve();  // これで Ei_h の DOF(_values) が埋まる :contentReference[oaicite:1]{index=1}      
      }
      */

      auto func_curlEi_phi = zComp(curl(Ei_h));              // Field -> Function
      auto func_Hi_phi   = -(1.0 / (mu0 * omega)) * func_curlEi_phi;   // degree 1

      // --- Hphi_i (Function) を H1スカラー場へ射影して Wall 上で評価できるようにする ---
      field::Field<C, form::Form0> Hphi_h(
        "Hphi_mode_" + std::to_string(i),
        domE,
        functionSpaceH1::HierarchicalH1,
        pOrder
      );

      {
        problem::Formulation<C> projH("L2proj_Hphi_mode_" + std::to_string(i));   
              
        projH.integral(wAxiC * dof(Hphi_h), tf(Hphi_h), Omega, gVol);
        projH.integral(wAxiC * func_Hi_phi,    tf(Hphi_h), Omega, gVol);        

        projH.pre();
        projH.assemble();
        projH.solve();
      }


      auto Hnorm2_h = Hphi_h * conj(Hphi_h);

      const auto Ez_i  = yComp(Ei_h);
      const auto Er_i  = xComp(Ei_h);
      const auto Enorm2_h = Ei_h * conj(Ei_h); // |E|^2

      // Stored energy:
      //   U = (1/4) ∫ (ε|E|^2 + μ|H|^2) dV
      // with dV = 2π r dA => weight wAxi
      //
      // NOTE: post::integrate vs post::integral naming differs by version.
      // Replace "post::integrate" with the correct function in your build.      
      C U = post::integrate(
        wAxiC * C(eps0 / 4.0) * Enorm2_h +
        wAxiC * C(mu0  / 4.0) * Hnorm2_h,
        Omega, gVol
      );

      // Accelerating voltage along axis:
      //   Vacc = ∫ Ez(r=0,z) dz
      C Vacc = post::integrate(Ez_i, Axis, gLin);

      // Wall loss (surface resistance):
      //   Rs = sqrt( ω μ0 / (2 σ) )
      //   Ploss = (1/2) ∫ Rs |Ht|^2 dS, dS = 2π r ds
      const double Rs = std::sqrt(omega * mu0 / (2.0 * sigma_wall));
      C Ploss = post::integrate(
        wAxiC * C(0.5 * Rs) * Hnorm2_h,        
        Wall, gLin
      );

      const double Q0     = omega * std::real(U) / std::real(Ploss);
      const double RoverQ = (std::real(Vacc) * std::real(Vacc)) / (omega * omega * std::real(U));

      std::cout
        << "U=" << U
        << " Ploss=" << Ploss
        << " Vacc=" << Vacc
        << " Q0=" << Q0
        << " (R/Q)=" << RoverQ
        << "\n";

      // -----------------------------
      // 3) Export views (like .pos)
      // -----------------------------
      // GmshFEM examples show post::save(...) usage 
      //lambda.save("pillbox_m0_lambda");      
      //post::save(Ei_h,     Omega, "pillbox_m0_E");
      //post::save(Ez_i,    Axis,  "pillbox_m0_Ez");
      //post::save(Er_i,    Omega,  "pillbox_m0_Er");
      post::save(Hphi_h,  Omega, "pillbox_m0_Hphi");
      post::save(Hnorm2_h,  Wall, "pillbox_m0_Hnorm2");
      
    }
  }
  
  /*
  C Vcheck = post::integrate(
    wAxiC * C(1.0 / (M_PI * R * R * L)),
    Omega, gVol
  );
  C Scheck = post::integrate(
    wAxiC * C(1.0 / (2.0 * M_PI * R * R + 2.0 * M_PI * R * L)),
    Wall, gLin
  );

  std::cout << "Vcheck=" << Vcheck << " Scheck=" << Scheck << "\n";
  */

  return 0;
}
