#include "../include/random_bit.h"
#include "gtest/gtest.h"
#include <bitset>

namespace math
{
	namespace random
	{
		TEST(RandomBit, test)
		{
			RandomBit *rand;
			uint32_t iseed = 1;
			uint32_t iseed2 = 1;
			int bit, bit2;

			for (uint32_t i = 0; i < 100; i++)
			{
				bit = rand->irbit(iseed);
				bit2 = rand->irbit_register_representation(iseed2);
				std::cout << "i: " << i << " (iseed,bit) = (" << iseed << "," << bit << ") (" << iseed2 << "," << bit2 << ")" << std::endl;
			}
			uint32_t x = 1;
			std::cout << ((x << 1) | 1) << std::endl;
			// EXPECT_EQ(a, golden);
		}

		TEST(RandomBit16, test)
		{
			RandomBit16 *rand16;
			uint16_t iseed = 1;
			std::bitset<16> bitset1 = iseed;
			std::cout << bitset1 << std::endl;
			uint16_t bit;
			for (uint32_t i = 0; i < 100; i++)
			{
				bit = rand16->irbit(iseed);
				bitset1 = iseed;
				std::cout << bitset1 << std::endl;
				std::cout << "i: " << i << " iseed: " << iseed << " bit: " << bit << std::endl;
			}
		}

		TEST(RandomBit16, test2)
		{
			RandomBit16 *rand16;
			uint16_t iseed = 1;
			std::bitset<16> bitset;
			for (int i = 0; i < 100; i++)
			{
				rand16->irbit2(iseed);
				bitset = iseed;

				std::cout << bitset << std::endl;
			}
		}

	} // namespace random

} // namespace math