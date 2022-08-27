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

		int RandomBit::irbit1(uint32_t &iseed)
		{
			uint32_t newbit;

			newbit = (iseed & IB18) >> 17 ^ (iseed & IB5) >> 4 ^ (iseed & IB2) >> 1 ^ (iseed && IB1);
			iseed = (iseed << 1) | newbit;
			return (int)newbit;
		};
	} // namespace random

} // namespace math
