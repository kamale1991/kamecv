#include "../include/random_bit.h"

namespace math
{
	namespace random
	{

		RandomBit::RandomBit(/* args */)
		{
		}

		RandomBit::~RandomBit()
		{
		}

		uint32_t RandomBit::irbit_register_representation(uint32_t &iseed)
		{
			uint32_t newbit;

			newbit = (iseed & IB18) >> 17 ^ (iseed & IB5) >> 4 ^ (iseed & IB2) >> 1 ^ (iseed & IB1);
			iseed = (iseed << 1) | newbit;
			return newbit;
		};

		uint32_t RandomBit::irbit(uint32_t &iseed)
		{
			if (iseed & IB18)
			{
				std::cout << ((iseed ^ MASK) << 1) << std::endl;
				iseed = ((iseed ^ MASK) << 1) | IB1;
				return 1;
			}
			else
			{
				iseed <<= 1;
				return 0;
			}
		}

		uint16_t RandomBit16::irbit(uint16_t &iseed)
		{
			if (iseed & IB16)
			{
				iseed = ((iseed ^ MASK) << 1) | IB1;
				return 1;
			}
			else
			{
				iseed <<= 1;
				return 0;
			}
		};

		uint32_t RnadomBit32::irbit(uint32_t &iseed)
		{
			uint32_t bit;
			bit = (iseed & IB2) ^ (iseed & IB3);
			printf("%04X %d\n", iseed, bit);
			return bit;
		}

		uint16_t RandomBit16::irbit2(uint16_t &iseed)
		{
			uint16_t bit;
			bit = (iseed & 0x0001) ^ ((iseed & 0x0004) >> 2) ^ ((iseed & 0x0008) >> 3) ^ ((iseed & 0x0020) >> 5);
			iseed = (iseed >> 1) | (bit << 15);
			printf("%04X %d\n", iseed, bit);
			return bit;
		}

	} // namespace random

} // namespace math
