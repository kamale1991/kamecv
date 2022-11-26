#include <iostream>

namespace math
{
	class Sort
	{
	private:
		/* data */
	public:
		Sort(/* args */){};
		~Sort(){};

		void piksrt(int n, float *array);
		void sort(int n, float *ra);

		void swap(int ia, int ib, float *array);
		void heapsrt(int n, float *array);
		void upheap(int n, float *array);
		void downheap(int n, float *array);
	};

} // namespace math
