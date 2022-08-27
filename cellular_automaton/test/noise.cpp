#include "gtest/gtest.h"
#include "../include/noise.h"

TEST(generate_gaussian_random_array, test)
{

    int size = 100;
    float *data = (float *)malloc(sizeof(float) * size);
    if (data == NULL)
    {

        printf("Memory cannot be allocated.");
    }
    else
    {
        printf("Memory has been allocated. The address is %p\n", data);
    }
    float mu = 0.0;
    float sigma = 10.0;

    CA::generate_rondom_gauss_array(mu, sigma, size, data);

    for (int i = 0; i < size; i++)
    {
        std::cout << data[i] << std::endl;
    }

    free(data);
}