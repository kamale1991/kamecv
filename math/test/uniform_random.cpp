#include "../include/uniform_random.h"
#include "gtest/gtest.h"
#include <vector>
#include <numeric>

namespace math
{
	namespace random
	{
		TEST(UniformRandom, test)
		{
			UniformRandom *urand = new UniformRandom();
			int32_t iseed = 1;
			float result = 0.;
			std::vector<float> vec;

			for (int i = 0; i < 1000000; i++)
			{
				result = urand->frand(iseed);
				vec.push_back(result);
			}

			float average = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
			EXPECT_NEAR(0.5, average, 1.0e-4);
		}
	} // namespace random

} // namespace math
