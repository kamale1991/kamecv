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

		int RandomBit::irbit_register_representation(uint32_t &iseed)
		{
			uint32_t newbit;

			newbit = (iseed & IB18) >> 17 ^ (iseed & IB5) >> 4 ^ (iseed & IB2) >> 1 ^ (iseed && IB1);
			iseed = (iseed << 1) | newbit;
			return (int)newbit;
		};

		int RandomBit::irbit(uint32_t &iseed)
		{
			if (iseed & IB18)
			{
				iseed = ((iseed ^ MASK) << 1) | IB1;
				return 1;
			}
			else
			{
				iseed <<= 1;
				return 0;
			}
		}

	} // namespace random

} // namespace math
