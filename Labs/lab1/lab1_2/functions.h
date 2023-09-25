#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>


#define MIN_EPSILON 1e-9



//digit_check
enum digit_check_status_codes
{
    overflow,
    underflow,
    digit,
    not_digit,
};

enum digit_check_status_codes overflow_underflow_normal (const long double value)
{
    if (value >= LFONG_MAX)
    {
        return overflow;
    }
    else if (value <= LDBL_MIN)
    {
        return underflow;
    }
    return digit;
}

enum digit_check_status_codes is_digit(const char *str, long double *value_from_string) {
    char *endptr;
    *value_from_string = strtolf(str, &endptr, 10); 
    if (*str != '\0' && *endptr == '\0')
    {
        return overflow_underflow_normal(*value_from_string);
    }
    value_from_string = NULL;
    return not_digit;
}
//-------------------------------------


long double bpow(long double base, long long degree)
{
    long double result = 1.0L;
    while (degree > 0) {
        if(degree & 1)
        {
            result = (result * base);
        }
        base = (base * base);
        degree = degree >> 1;
    }
    return result;   
}

enum epsilon_status_codes
{
    good_epsilon,
    big_epsilon,
    very_small_epsilon
};

enum epsilon_status_codes check_epsilon(const long double epsilon)
{
    if (epsilon < MIN_EPSILON)
    {
        return very_small_epsilon;
    }
    else if (epsilon >= 1)
    {
        return big_epsilon;
    }
    return good_epsilon;
}

enum epsilon_status_codes Euler_number_by_limit (long double epsilon)
{
    
    long double e = 1.0L, division = 1.0L;
    long long n = 1;
    while (division > epsilon || division > MIN_EPSILON)
    {
        n++;
        division = 1.0L / n;
    }
    e = bpow(e + division, n);
    printf("%lld\n", n);
    return e;   
}

#endif