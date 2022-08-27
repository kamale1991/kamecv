#include <iostream>
#include <random>
#include "../include/matplotlibcpp.h"

namespace CA
{
    void generate_rondom_gauss_array(float mu, float sigma, int length, float *data)
    {
        // 非決定的乱数をシードに使用したメルセンヌツイスタ
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::cout << 0xffffffff << std::endl;
        std::normal_distribution<> norm(mu, sigma);
        for (int i = 0; i < length; i++)
        {
            data[i] = norm(mt);
        }
    }
}