#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAX_RANGE 1e10

bool has_finite_representation(double number, int base) 
{
    long long divider = 1, iteration = 0;
    while (number != (int)number)
    {
        if (number > MAX_RANGE || divider >= 1e18 || iteration > 1e8) return false;
        number *= base;
        divider *= base;
        iteration++;
    }

    for (int i = 2; i <= divider && i <= base; i++) 
    {
        while (base % i == 0) 
        {
            base /= i;
            while (divider % i == 0)
            {
                divider /= i;
            }
        }
    }

    return divider == 1;
}

void check_inf_or_not(int base, int count, ...) 
{
    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) 
    {
        double dnumber = va_arg(ptr, double);
        printf("Representation %lf in base %d: %s\n", dnumber, base, has_finite_representation(dnumber, base) ? "Finite" : "Infinite");
    }

    va_end(ptr);
}