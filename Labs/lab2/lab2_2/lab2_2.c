#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

long double fpow(long double a, int n)
{
    if (n < 0) return 1.0L / fpow(a, abs(n));
    if (n == 0) return 1.0L;
    if (n % 2) return a * fpow(a, n - 1);
    else 
    {
        long double x = fpow(a, n * 0.5);
        return x * x;
    }
}

long double geometry_aver(int count, ...)
{
    va_list ptr;
    va_start(ptr, count);
    long double result = 1.0L;
    for (int i = 0; i < count; i++)
    {
        result *= va_arg(ptr, long double);
    }
    va_end(ptr);
    return powl(result, 1.0L / count);
}

int main()
{    
    printf("Answer for 1 task is %.15Lf\n", geometry_aver(1, (long double)-5));
    printf("Answer for 2 task is %.15Lf\n", fpow((long double)123.123, 3));
}