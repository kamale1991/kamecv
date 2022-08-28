#include "../include/random_bit.h"
#include "gtest/gtest.h"
#include <bitset>

namespace math
{
	namespace random
	{
		TEST(RandomBit16, test)
		{
			RandomBit16 *rand16 = new RandomBit16();
			uint16_t iseed = 1;
			std::bitset<16> bitset1 = iseed;
			std::cout << bitset1 << std::endl;
			uint16_t bit;
			for (uint32_t i = 0; i < 100; i++)
			{
				bit = rand16->irbit(iseed);
				bitset1 = iseed;
			}
		}

		TEST(RandomBit16, test2)
		{
			RandomBit16 *rand16 = new RandomBit16();
			uint16_t iseed = 1;
			std::bitset<16> bitset;
			for (int i = 0; i < 100; i++)
			{
				rand16->irbit2(iseed);
				bitset = iseed;
			}
		}

	} // namespace random

} // namespace math