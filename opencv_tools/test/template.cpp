#include "../include/template.h"
#include "gtest/gtest.h"

TEST(sample_template, test)
{
    // int a = fibonacci(3);
    int a = 5;
    int golden = 5;
    EXPECT_EQ(a, golden);
}