#include <iostream>

namespace math
{
	namespace random
	{
		class RandomBit
		{
		private:
			/* data */
			int IB1 = 1;
			int IB2 = 2;
			int IB5 = 16;
			int IB18 = 131072;

		public:
			RandomBit(/* args */);
			~RandomBit();
			int irbit1(uint32_t &iseed);
		};

	} // namespace random

} // namespace math