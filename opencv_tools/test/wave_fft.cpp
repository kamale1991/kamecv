#include <opencv2/opencv.hpp>
#include "gtest/gtest.h"

#include <string>
// figures/fft/waves.tifを読み込んで開く
using namespace cv;

TEST(wave_fft, open)
{
    std::string filename = "figures/fft/waves.tif";
    Mat image = imread(filename);
    imshow("waves", image);
    // waitKey(0);
}

TEST(wave_fft, test)
{
    std::string filename = "figures/fft/waves.tif";
    Mat image = imread(filename, IMREAD_GRAYSCALE);

    Mat padded;
    // 入力画像をDFTに最適な大きさに広げる。
    // 画像サイズ2,3,5の倍数のときに高速になる.
    int m = getOptimalDFTSize(image.rows);
    int n = getOptimalDFTSize(image.cols);

    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;

    merge(planes, 2, complexI);

    dft(complexI, complexI);

    // log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                    // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]); // planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1); // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or colmns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, NORM_MINMAX);

    imshow("Input Image", image);
    waitKey(0);
    imshow("spectrum magnitude", magI);
    waitKey(0);
}

TEST(wave_fft, molphology)
{
    int morph_elem = 0;
    int morph_size = 0;
    int morph_operator = 0;
    int const max_operator = 4;
    int const max_elem = 2;
    int const max_kernel_size = 21;
    const char *window_name = "Morphology Transformations Demo";
}