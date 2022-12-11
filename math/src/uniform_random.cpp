#include "../include/uniform_random.h"

namespace math
{
	UniformRandom::UniformRandom(/* args */)
	{
		ia = 16807;
		im = 2147483647;
		am = 1.0 / (float)im;
		iq = 127773;
		ir = 2836;
		ntab = 32;
		ndiv = 1 + (im - 1) / ntab;
		eps = 1.2e-7;
		rnmx = 1.0 - eps;
		iy = 0;
		for (int i = 0; i < 32; i++)
		{
			iv[i] = 0;
		}
	}

	UniformRandom::~UniformRandom()
	{
	}

	float UniformRandom::frand(int32_t &iseed)
	{
		int32_t i, k;
		float temp;
		if (iseed <= 0 || !iy)
		{
			if (-iseed < 1)
			{
				iseed = 1;
			}
			else
			{
				iseed = -iseed;
			}

			for (int32_t j = ntab + 7; j >= 0; j--)
			{
				k = iseed / iq;
				iseed = ia * (iseed - k * iq) - ir * k;
				if (iseed < 0)
				{
					iseed += im;
				}
				if (j < ntab)
				{
					iv[j] = iseed;
				}
			}
			iy = iv[0];
		}

		k = iseed / iq;
		iseed = ia * (iseed - k * iq) - ir * k;

		if (iseed < 0)
		{
			iseed += im;
		}

		i = iy / ndiv;
		iy = iv[i];

		iv[i] = iseed;
		if ((temp = am * iy) > rnmx)
		{
			return rnmx;
		}
		else
		{
			return temp;
		}
	}

} // namespace math
