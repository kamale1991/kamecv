#include "../include/sort.h"

namespace math
{
	void Sort::piksrt(int n, float *array)
	{
		float a;
		int j;
		for (int i = 1; i < n; i++)
		{
			a = array[i];
			j = i - 1;
			while (j >= 0 && array[j] > a)
			{
				array[j + 1] = array[j];
				j--;
			}
			array[j + 1] = a;
		}
	};
} // namespace math
