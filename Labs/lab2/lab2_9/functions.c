#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_RANGE 1e10

typedef struct 
{
    bool inf;
    int base;
    double num;
} Numb;

void print_arr (Numb * arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Number : %f; base: %d; status: %s", arr[i].num, arr[i].base, (!arr[i].inf) ? "Finite" : "Infinite");
        printf("\n");
    }
    printf("\n");
}

bool has_finite_representation(double number, int base) 
{
    long long divider = 1, iteration = 0;
    while (number != (int)number)
    {
        if (number > MAX_RANGE || divider >= 1e18 || iteration > 1e6) return false;
        number *= base;
        divider *= base;
        iteration++;
    }

    for (int i = 2; i <= base; i++) 
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

void check_inf_or_not(Numb ** mas, int base, int count, ...) 
{
    *mas = (Numb * )malloc(sizeof(Numb) * count);
    if (mas == NULL) 
    {

        return;
    }
    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) 
    {
        double dnumber = va_arg(ptr, double);
        if(has_finite_representation(dnumber, base))
        {
            (*mas)[i].inf = false;
        }
        else
        {
            (*mas)[i].inf = true;
        }
        (*mas)[i].base  = base;
        (*mas)[i].num  = dnumber;
    }
    
    va_end(ptr);
}