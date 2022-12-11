#include "../include/tridiagonal.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(Tridiagonal, n_3)
{
	// A.u = R
	// A = {{2, 1, 0}, {1, 2, 1}, {0, 1, 2}}
	// R = {1, 2, 3}
	// - answer
	// u = {1/2, 0, 3/2}

	float a[3] = {0, 1, 1};
	float b[3] = {2, 2, 2};
	float c[3] = {1, 1, 0};
	float u[3] = {0, 0, 0};
	float r[3] = {1, 2, 3};
	const uint64_t n = 3;
	math::tridiagonal(a, b, c, u, r, n);
	std::cout << "answer: ";
	float ugolden[3] = {0.5, 0.0, 1.5};
	for (int i = 0; i < n; i++)
	{
		std::cout << u[i] << " ";
		EXPECT_NEAR(ugolden[i], u[i], 1e-4);
	}
	std::cout << std::endl;
};