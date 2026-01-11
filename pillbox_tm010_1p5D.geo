// pillbox_tm010_1p5D.geo
// 1.5D (Fourier in phi) 用の軸対称形状： (z,r) 断面メッシュ
// Units: meters.
//
// ★座標の取り方を変更★
//   x = z (axial), y = r (radial)
//   → 軸 (r=0) は y=0 の線
//
// TM010 pillbox frequency (vacuum) depends only on radius:
//   f = x01 * c0 / (2*pi*R), where x01 = first zero of J0 = 2.4048255577
//
// GetDP 側で 2.5D(=フーリエ次数 n) を回す場合も、形状はこの2D断面でOK。
// （n依存は .pro 側の弱形式/ヤコビアンで入れる）
//
// gmsh pillbox_tm010_1p5D.geo -3 -format msh2 -o pillbox_tm010_1p5D.msh
//

SetFactory("OpenCASCADE");

// -------------------- Parameters --------------------
f0  = 5.712e9;             // target frequency [Hz] (reference)
c0  = 299792458;           // speed of light [m/s]
x01 = 2.404825557695773;   // J0 first root

R = x01 * c0 / (2*Pi*f0);  // ~ 20.088 mm for 5.712 GHz
L = c0/(2*f0);             // example length: lambda/2 ~ 26.24 mm (edit freely)

lc = 1.0e-3;              // target mesh size [m]

// -------------------- Geometry (rectangle in z-r plane) --------------------
// Coordinate convention:
//   y = z (axial), x = r (radial)

Point(1) = {0, 0, 0, lc};  // (z=0, r=0)
Point(2) = {L, 0, 0, lc};  // (z=L, r=0)
Point(3) = {L, R, 0, lc};  // (z=L, r=R)
Point(4) = {0, R, 0, lc};  // (z=0, r=R)

// Boundary lines (counter-clockwise)
Line(1) = {1, 4};   // z = 0  (end plate)
Line(2) = {4, 3};   // r = R  (side wall)
Line(3) = {3, 2};   // z = L  (end plate)
Line(4) = {1, 2};   // r = 0  (Axis)

Line Loop(1) = {1, 2, 3, -4};
Plane Surface(1) = {1};

// -------------------- Physical groups --------------------
// These tags become Region numbers in GetDP.
//
// Surface(101): 計算領域（真空）
// Line(201): PEC壁（z=0, z=L, r=R）
// Line(202): 軸（r=0）…2.5Dの正則条件/評価用

Physical Surface(101) = {1};      // Omega (cavity vacuum region)

// PEC walls (exclude axis)
Physical Line(201) = {1,2,3};     // Walls (PEC)

// Axis
Physical Line(202) = {4};         // Axis (r=0)

// （任意）壁を分割して使いたい場合：GetDP側で損失や境界条件を分ける用途
// Physical Line(211) = {4};       // z=0 end plate
// Physical Line(212) = {2};       // z=L end plate
// Physical Line(213) = {3};       // r=R side wall

// Mesh options (optional)
// Mesh.Algorithm = 6; // Frontal-Delaunay for 2D
