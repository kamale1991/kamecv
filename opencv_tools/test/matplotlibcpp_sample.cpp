#include "../include/matplotlibcpp.h"
#include <vector>
#include "gtest/gtest.h"

namespace plt = matplotlibcpp;

TEST(matplotlib, sample)
{
    std::vector<double> x(100), y(100);

    for (int i = 0; i < 100; i++)
    {
        x[i] = 1.0 * i / 100;
        y[i] = x[i] * x[i];
    }

    plt::plot(x, y);

    plt::save("./sample1.png");
};