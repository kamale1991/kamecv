/**
 * @file tridiagonal.h
 * @author kamale
 * @brief
 * @version 0.1
 * @date 2022-10-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdint.h>
#include <iostream>
namespace math
{

	void tridiagonal(float *a, float *b, float *c, float *u, float *r, uint64_t n)
	{

		float arr_btilde[n];
		float rbar = r[0];
		float btilde = 1.0 / b[0];
		arr_btilde[0] = btilde;
		// previous rbar and btilde
		float rbar_i_1 = rbar;
		float btilde_i_1 = btilde;
		u[0] = rbar * btilde;
		for (int i = 1; i < n; i++)
		{
			rbar = r[i] - a[i] * rbar_i_1 * btilde_i_1;
			btilde = 1.0 / (b[i] - a[i] * c[i - 1] * btilde_i_1);
			arr_btilde[i] = btilde;
			rbar_i_1 = rbar;
			btilde_i_1 = btilde;
			u[i] = rbar * btilde;
		}

		for (int i = n - 2; i >= 0; i--)
		{
			u[i] -= a[i + 1] * arr_btilde[i] * u[i + 1];
		}
	}
} // namespace math
