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

			newbit = iseed & IB18 >> 17 ^

							 return 1;
		};
	} // namespace random

} // namespace math
