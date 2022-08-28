#include "../include/random_bit.h"
#include "gtest/gtest.h"

namespace math
{
	namespace random
	{
		TEST(RandomBit, test)
		{
			RandomBit *rand;
			uint32_t iseed = 1;
			int res = rand->irbit1(iseed);
			std::cout << "iseed: " << iseed << " result: " << res << std::endl;
			// EXPECT_EQ(a, golden);
		}
	} // namespace random

} // namespace math