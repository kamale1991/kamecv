#include <iostream>
#include <Eigen/Dense> // Eigen を使用するために必要なヘッダーファイル
#include <cmath>

namespace ChatGPTAnswer
{

	void diagonalize()
	{
		// 行列 A を定義する
		Eigen::MatrixXd A(2, 2);
		A << 1, 2,
				3, 4;

		// 行列 A の特異値分解を行う
		Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);

		// 特異値分解の結果を取得する
		Eigen::MatrixXd U = svd.matrixU();
		Eigen::MatrixXd D = svd.singularValues().asDiagonal();
		Eigen::MatrixXd V = svd.matrixV();

		// 対角行列 D の固有値を表示する
		std::cout << "Eigenvalues of A:" << std::endl;
		std::cout << D << std::endl;

		return 0;
	}

	// 5x5のガウシアンフィルタを適用する関数
	void applyGaussianFilter(float **array, int N, int M)
	{
		// 5x5のガウシアンフィルタの定義
		const int filterSize = 5;
		const float filter[filterSize][filterSize] = {
				{1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0},
				{4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0},
				{6 / 256.0, 24 / 256.0, 36 / 256.0, 24 / 256.0, 6 / 256.0},
				{4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0},
				{1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0}};
		// 一時的に平滑化後の値を保存する配列
		float temp[N][M];
		// 全ての要素に対して、5x5のガウシアンフィルタを適用する
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				// 周りの要素の合計値を求める
				float sum = 0;
				for (int k = 0; k < filterSize; ++k)
				{
					for (int l = 0; l < filterSize; ++l)
					{
						// 範囲外の場合はフィルタ係数をかけない
						if (i + k - filterSize / 2 < 0 || i + k - filterSize / 2 >= N || j + l - filterSize / 2 < 0 || j + l - filterSize / 2 >= M)
						{
							continue;
						}
						sum += array[i + k - filterSize / 2][j + l - filterSize / 2] * filter[k][l];
					}
				}
				// 平滑化後の値を配列に保存する
				temp[i][j] = sum;
			}
		}
	}

	// 3x3の構造体要素{{0,1,0}, {0, 1, 0}, {0, 1, 0}}でerosionを行う関数
	// 単なる畳み込み処理をしているだけ。
	void applyErosion(float **input, float **output, int N, int M)
	{
		// 3x3の構造体要素の定義
		const int filterSize = 3;
		const float filter[filterSize][filterSize] = {
				{0, 1, 0},
				{0, 1, 0},
				{0, 1, 0}};
		// 全ての要素に対して、3x3の構造体要素を適用する
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				// 周りの要素の合計値を求める
				float sum = 0;
				for (int k = 0; k < filterSize; ++k)
				{
					for (int l = 0; l < filterSize; ++l)
					{
						// 範囲外の場合はフィルタ係数をかけない
						if (i + k - filterSize / 2 < 0 || i + k - filterSize / 2 >= N || j + l - filterSize / 2 < 0 || j + l - filterSize / 2 >= M)
						{
							continue;
						}
						sum += input[i + k - filterSize / 2][j + l - filterSize / 2] * filter[k][l];
					}
				}
				// erosion後の値を配列に保存する
				output[i][j] = sum;
			}
		}
	}

	// ヘッセ行列を計算して、変化量が大きいピクセルをエッジとして抽出する関数
	// ヘッセ行列が何かは知らないらしい。x方向とy方向の差分を計算してその二乗和が閾値を超えた場合に元の値を残すコードになっている。
	void detectEdges(uint16_t **input, uint16_t **output, int N, int M)
	{
		// 全てのピクセルに対して、ヘッセ行列を計算する
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				// ヘッセ行列を求める
				int dx = input[i][j + 1] - input[i][j - 1];
				int dy = input[i + 1][j] - input[i - 1][j];
				int magnitude = sqrt(dx * dx + dy * dy);
				// 変化量が大きいピクセルをエッジとして抽出する
				if (magnitude > THRESHOLD)
				{
					output[i][j] = input[i][j];
				}
				else
				{
					output[i][j] = 0;
				}
			}
		}
	}

#if 0 // FFTを使用した畳み込み処理

	// 2次元のガウシアンフィルタ
const int GAUSSIAN_SIZE = 5;
const double GAUSSIAN[GAUSSIAN_SIZE][GAUSSIAN_SIZE] = {
    { 1.0 / 256,  4.0 / 256,  6.0 / 256,  4.0 / 256, 1.0 / 256 },
    { 4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256 },
    { 6.0 / 256, 24.0 / 256, 36.0 / 256, 24.0 / 256, 6.0 / 256 },
    { 4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256 },
    { 1.0 / 256,  4.0 / 256,  6.0 / 256,  4.0 / 256, 1.0 / 256 }
};

// 入力画像(inImage)と出力画像(outImage)のサイズ
const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 480;

// FFTに必要な配列
std::complex<double> in[IMAGE_HEIGHT][IMAGE_WIDTH];
std::complex<double> out[IMAGE_HEIGHT][IMAGE_WIDTH];

// 畳み込み処理を行う
for (int y = 0; y < IMAGE_HEIGHT; ++y) {
    for (int x = 0; x < IMAGE_WIDTH; ++x) {
        // 入力画像を複素数の配列に格納
        in[y][x] = std::complex<double>(inImage[y][x], 0);
    }
}

// 高速フーリエ変換
fft2d(in, out, IMAGE_WIDTH, IMAGE_HEIGHT);

// ガウシアンフィルタを複素数の配列に格納
std::complex<double> filter[GAUSSIAN_SIZE][GAUSSIAN_SIZE];
for (int y = 0; y < GAUSSIAN_SIZE; ++y) {
    for (int x = 0; x < GAUSSIAN_SIZE; ++x) {
        filter[y][x] = std::complex<double>(GAUSSIAN[y][x], 0);
    }
}

// 高速フーリエ変換
fft2d(filter, filter, GAUSSIAN_SIZE, GAUSSIAN_SIZE);

//

#endif

	void erosion()
	{

		// 入力画像のビットマップデータ
		uint8_t input[320 * 240];

		// 出力画像のビットマップデータ
		uint8_t output[320 * 240];

		// 画像の幅と高さ
		int width = 320, height = 240;

		// 画像の各ピクセルについて処理を行う
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// 入力画像の周りのピクセルを取得する
				uint8_t pixels[] = {
						input[y * width + x],
						input[y * width + (x + 1) % width],
						input[y * width + (x - 1 + width) % width],
						input[((y + 1) % height) * width + x],
						input[((y - 1 + height) % height) * width + x]};

				// 入力画像の周りのピクセルの最小値を出力画像に描画する
				output[y * width + x] = *std::min_element(std::begin(pixels), std::end(pixels));
			}
		}
		// 上記のコードでは、入力画像の各ピクセルの周りにあるピクセルの最小値を出力画像に描画しています。Erosion演算では、入力画像の各ピクセルの周りにあるピクセルの値がすべて0の場合、そのピクセルを0にします。そのため、上記のコードでは、ピクセルの周りにあるピクセルの最小値が0の場合、そのピクセルを0に描画しています。

		// Erosion演算では、ピクセル周りのピクセルを取得するために、画像の周りを参照する必要があります。上記のコ
	}

	// C++でビットマップから輪郭抽出を行うコードは次のようになります。
	void hessian()
	{
		// 画像の各ピクセルについて処理を行う
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// Hessian行列を求めるための入力画像の周りのピクセルを取得する
				uint8_t pixels[] = {
						input[y * width + x],
						input[y * width + (x + 1) % width],
						input[y * width + (x - 1 + width) % width],
						input[((y + 1) % height) * width + x],
						input[((y - 1 + height) % height) * width + x],
						input[((y + 1) % height) * width + (x + 1) % width],
						input[((y + 1) % height) * width + (x - 1 + width) % width],
						input[((y - 1 + height) % height) * width + (x + 1) % width],
						input[((y - 1 + height) % height) * width + (x - 1 + width) % width]};

				// Hessian行列を求める
				int hessian[2][2] = {
						{pixels[2] + pixels[4] - 2 * pixels[0] + pixels[6] + pixels[8],
						 pixels[1] + pixels[3] - 2 * pixels[0] + pixels[5] + pixels[7]},
						{pixels[1] + pixels[3] - 2 * pixels[0] + pixels[5] + pixels[7],
						 pixels[2] + pixels[4] - 2 * pixels[0] + pixels[6] + pixels[8]}};
			}
			// 入力画像の周りのピクセルから計算されたHessianが閾値以上の場合、出力画像を1にする
			if (hessian >= THRESHOLD)
			{
				output[y * width + x] = 1;
			}
		}
	}

	void corner_sobel()
	{
		// 入力画像のビットマップデータ
		uint8_t input[320 * 240];

		// 出力画像のビットマップデータ
		uint8_t output[320 * 240];

		// 画像の幅と高さ
		int width = 320, height = 240;

		// 画像の各ピクセルについて処理を行う
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// 入力画像の周りのピクセルを取得する
				uint8_t pixels[] = {
						input[y * width + x],
						input[y * width + (x + 1) % width],
						input[y * width + (x - 1 + width) % width],
						input[((y + 1) % height) * width + x],
						input[((y - 1 + height) % height) * width + x]};

				// 入力画像の周りのピクセルからSobelフィルタを計算する
				int sobel_x = pixels[1] + 2 * pixels[3] + pixels[4] - pixels[2] - 2 * pixels[0];
				int sobel_y = pixels[2] + 2 * pixels[1] + pixels[3] - pixels[4] - 2 * pixels[0];

				// 入力画像の周りのピクセルから計算されたSobelフィルタの値から、コーナーの検出値を計算する
				int corner = sobel_x * sobel_x + sobel_y * sobel_y;

				// 入力画像の周りのピクセルから計算されたコーナーの検出値が閾値以上の場合、出力画像を1にする
				if (corner >= THRESHOLD)
				{
					output[y * width + x] = 1;
				}
			}
		}
	}

	// フーリエ変換を行う関数
	std::vector<std::complex<double>> fft(const std::vector<std::complex<double>> &x)
	{
		// xの長さが1ならそのまま返す
		if (x.size() == 1)
			return x;

		// xを偶奇で分ける
		std::vector<std::complex<double>> even, odd;
		for (int i = 0; i < x.size(); i++)
		{
			if (i % 2 == 0)
				even.push_back(x[i]);
			else
				odd.push_back(x[i]);
		}

		// 偶奇で分けた信号を再帰的にフーリエ変換する
		std::vector<std::complex<double>> f_even = fft(even);
		std::vector<std::complex<double>> f_odd = fft(odd);

		// 結果を格納するための変数
		std::vector<std::complex<double>> result(x.size());

		// フーリエ変換の計算
		for (int k = 0; k < x.size() / 2; k++)
		{
			std::complex<double> tmp = std::polar(1.0, -2.0 * M_PI * k / x.size()) * f_odd[k];
			result[k] = f_even[k] + tmp;
			result[k + x.size() / 2] = f_even[k] - tmp;
		}

		return result;
	}

	// シグモイド関数
	double sigmoid(double x)
	{
		return 1.0 / (1.0 + std::exp(-x));
	}

	// ニューラルネットワークを表す構造体
	struct NN
	{
		std::vector<std::vector<double>> w; // 重み
		std::vector<double> b;							// バイアス
		int n_layers;												// 層の数

		// コンストラクタ
		NN(int n_layers, int n_input, int n_output)
				: n_layers(n_layers), w(n_layers), b(n_layers)
		{
			// 入力層と出力層を除く各層のノード数
			int n_hidden = (n_input + n_output) / 2;

			// 重みとバイアスを初期化する
			for (int i = 0; i < n_layers; i++)
			{
				w[i].resize(n_hidden);
				b[i] = 0.0;
				for (int j = 0; j < n_hidden; j++)
				{
					w[i][j] = 0.0;
				}
			}
		}

		// フォワードプロパゲーション
		std::vector<double> forward(const std::vector<double> &x)
		{
			std::vector<double> y = x;
			for (int i = 0; i < n_layers; i++)
			{
				std::vector<double> z(y.size());
				for (int j = 0; j < y.size(); j++)
				{
					z[j] = 0.0;
					for (int k = 0; k < y.size(); k++)
					{
						z[j] += y[k] * w[i][k];
					}
					z[j] += b[i];
					y[j] = sigmoid(z[j]);
				}
			}
			return y;
		}
	};

}