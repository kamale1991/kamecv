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

	void Sort::swap(int ia, int ib, float *array)
	{
		float tmp = array[ia];
		array[ia] = array[ib];
		array[ib] = tmp;
	}

	void Sort::upheap(int n, float *array)
	{
		while (n > 0)
		{
			// parent
			int m = ((n + 1) / 2 - 1);

			if (array[m] < array[n])
			{
				swap(m, n, array);
			}
			else
			{
				break;
			}
			n = m;
		}
	};

	void Sort::downheap(int n, float *array)
	{
		int m = 0;
		int tmp = 0;
		for (;;)
		{
			int left_child = (m + 1) * 2 - 1;
			int right_child = (m + 1) * 2;

			if (left_child >= n)
			{
				break;
			}

			if (array[left_child] > array[tmp])
			{
				tmp = left_child;
			}

			if (right_child < n && array[right_child] > array[tmp])
			{
				tmp = right_child;
			}

			if (tmp == m)
			{
				break;
			}

			swap(tmp, m, array);

			m = tmp;
		}
	}

	void Sort::heapsrt(int n, float *array)
	{
		int i = 0;
		//ヒープ中要素、未処理のデータの先頭
		//        0   1   2     3   4   5
		// array [ ] [ ] [ ] / [ ] [ ] [ ]
		//        heap          unprocessed input

		while (++i < n)
		{
			upheap(i, array);
		}

		while (--i > 0)
		{
			swap(0, i, array);
			downheap(i, array);
		}
	}

	void Sort::sort(int n, float *ra)
	{

		int l, j, ir, i;
		float rra;

		l = (n >> 1);
		ir = n;

		std::cout << "l= " << l << " ir= " << ir << std::endl;
		for (;;)
		{
			if (l > 1)
			{
				rra = ra[--l];
				std::cout << "rra = ra[--l]" << std::endl;
			}
			else
			{
				rra = ra[ir];
				ra[ir] = ra[1];
				if (--ir == 1)
				{
					ra[1] = rra;
					return;
				}
			}

			i = l;
			j = l << 1;
			std::cout << "i= " << i << " j= " << j << " l= " << l << " ir= " << ir << " rra= " << rra << std::endl;

			for (int k = 0; k < n; k++)
			{
				std::cout << ra[k] << " ";
			}
			std::cout << std::endl;

			while (j <= ir)
			{
				if (j < ir && ra[j] < ra[j + 1])
					++j;
				if (rra < ra[j])
				{
					ra[i] = ra[j];
					j += (i = j);
				}
				else
					j = ir + 1;
			}
			std::cout << "ra[i]=rra i=" << i << " rra= " << rra << std::endl;
			ra[i] = rra;
		}
	};
} // namespace math
