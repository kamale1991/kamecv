#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "gtest/gtest.h"

using namespace cv;
using namespace std;
TEST(binary_image, test)
{
    Mat original_image;
    Mat grayscale_image;
    Mat binary_image;
    namedWindow("Original Image");
    namedWindow("Show Binary");
    original_image = imread("./opencv_sample/fruits.jpg");
    cvtColor(original_image, grayscale_image, COLOR_BGR2GRAY);
    threshold(grayscale_image,
              binary_image, 100, 255, THRESH_BINARY);
    imshow("Original Image", original_image);
    imshow("Show Binary", binary_image);
    waitKey(0);
};