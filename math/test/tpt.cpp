#include "../include/tpt.h"
#include "gtest/gtest.h"
#include "cmath"

TEST(TPT, test)
{
	Common::TPT tpt("test");
	double sum = 0.;
	double sign = 1.0;
	for (int i = 0; i < 1000000; i++)
	{
		sum += 1.0 / static_cast<double>(2 * i + 1) * sign;
		sign *= -1.0;
	}
	tpt.stop();
	std::cout << "sum - pi / 4= " << sum - M_PI_4 << std::endl;
};