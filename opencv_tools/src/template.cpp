#include "../include/template.h"

int fibonacci(int n)
{
    int32_t t1 = 0;
    int32_t t2 = 1;
    int32_t buff = 0;
    for (int i = 1; i <= n; ++i)
    {
        buff = t1 + t2;
        t1 = t2;
        t2 = buff;
    }
    return t2;
}
