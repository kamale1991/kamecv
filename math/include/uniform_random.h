#include <iostream>

namespace math
{
	namespace random
	{
		class UniformRandom
		{
		private:
			/* data */
			int32_t ia, im, iq, ir, ntab, ndiv;
			float am, eps, rnmx;
			int32_t iy;
			int32_t iv[32];

		public:
			UniformRandom(/* args */);
			~UniformRandom();

			float frand(int32_t &iseed);
		};

	} // namespace random

} // namespace math
