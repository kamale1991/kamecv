#include "../include/sort.h"
#include "../include/tpt.h"
#include "gtest/gtest.h"
using namespace math;

TEST(piksrt, test)
{
	Sort sort;

	int n = 3;
	float arr[3] = {0.1F, 0.2F, 0.3F};
	float arr2[3] = {0.3F, 0.2F, 0.1F};
	float golden[3] = {0.1F, 0.2F, 0.3F};

	sort.piksrt(n, arr);
	sort.piksrt(n, arr2);
	for (int i = 0; i < n; i++)
	{
		EXPECT_FLOAT_EQ(arr[i], golden[i]);
	}

	for (int i = 0; i < n; i++)
	{
		EXPECT_FLOAT_EQ(arr2[i], golden[i]);
	}
};

TEST(sort, test)
{
	Sort sort;

	int n = 4;
	float arr[4] = {0.1F, 0.2F, 0.3F, 0.4F};
	float arr2[4] = {0.4F, 0.3F, 0.2F, 0.1F};
	float golden[4] = {0.1F, 0.2F, 0.3F, 0.4F};

	sort.heapsrt(n, arr);
	sort.heapsrt(n, arr2);
	for (int i = 0; i < n; i++)
	{
		EXPECT_FLOAT_EQ(arr[i], golden[i]);
	}

	for (int i = 0; i < n; i++)
	{
		EXPECT_FLOAT_EQ(arr2[i], golden[i]);
	}
};

TEST(sort, tpt)
{
	int n = 10000;
	float array[n];
	for (int i = 0; i < n; i++)
	{
		array[i] = 1000.0 - 0.1 * i;
	}

	Sort sort;
	Common::TPT tpt("piksrt");
	sort.piksrt(n, array);
	tpt.stop();
};
