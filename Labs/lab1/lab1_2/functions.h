#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>


#define MIN_EPSILON 1e-9





// enum digit_check_status_codes
// {
//     overflow,
//     underflow,
//     digit,
//     not_digit,
// };

// enum digit_check_status_codes overflow_underflow_normal (const long double value)
// {
//     if (value >= LFONG_MAX)
//     {
//         return overflow;
//     }
//     else if (value <= LDBL_MIN)
//     {
//         return underflow;
//     }
//     return digit;
// }

//info print
void print_help(const char * programm) {

    printf("Use: %s <epsilon> or use %s -help to list all functions\n\n", programm, programm);
    printf("-help: Display this help message.\n\n");
    printf("-info: Display info message.\n\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_2\n\n");
}
//-----------------


//Input & epsilon check
enum flags_status_codes
{
    info_flag,
    help_flag,
    undefined_flag,
    error_input
};

enum flags_status_codes check_flags (const char * flag)
{
    if (strcmp(flag, "-info") == 0)
    {
        return info_flag;
    }
    else if (strcmp(flag, "-help") == 0)
    {
        return help_flag;
    }
    else if (flag[0] == '-') return undefined_flag;
    return error_input;
}


enum epsilon_status_codes
{
    good_epsilon,
    big_epsilon,
    very_small_epsilon,
    not_digit
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


enum epsilon_status_codes is_digit(const char *str, long double *value_from_string) {
    char *endptr;
    *value_from_string = strtold(str, &endptr); 
    if (*str != '\0' && *endptr == '\0')
    {
        return check_epsilon(*value_from_string);
    }
    value_from_string = NULL;
    return not_digit;
}


enum input_check_status_codes
{
    good_input,
    invalid_input
};

enum input_check_status_codes Input_checker(int argc, char* argv[], long double * epsilon, char** str_epsilon)
{
    if (argc == 2)
    {
        switch (is_digit(argv[1], epsilon))
        {
            case good_epsilon: 
                *str_epsilon = argv[1];
                return good_input;
            case big_epsilon:
                printf("Epsilon is very large. Range: (1e-9 < x < 1)\n");
                return invalid_input;
            case very_small_epsilon:
                printf("Epsilon is very small. Range: (1e-9 < x < 1)\n");
                return invalid_input;
            case not_digit:
                switch (check_flags(argv[1]))
                {
                case info_flag:
                    print_info();
                    break;
                case help_flag:
                    print_help(argv[0]);
                    break;
                case undefined_flag:
                    printf("An undefined flag was entered into the console. Try again or use a flag -help\n");
                    return invalid_input;
                case error_input:
                    printf("Error. You entered neither a number nor a flag. Use -help\n");
                    return invalid_input;
                }
            break;
        }
    }
    printf("Enter the second argument\n");
    return invalid_input;
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


//for Euler number
long double Euler_number_by_limit (long double epsilon)
{
    
    long double e = 1.0L, division = 1.0L;
    long long n = 1;
    while (division > epsilon)
    {
        n++;
        division = 1.0L / n;
    }
    e = bpow(e + division, n);
    return e;   
}

long double seriesEulerNumber(long double epsilon)
{
    long double e = 1.0L;
    long double term = 1.0L;
    unsigned long factorial = 1;
    int n = 1;


    while (term > epsilon)
    {
        term /= factorial;
        e += term;
        n++;
        factorial *= n;
    }

    return e;
}

void print_E (long double epsilon)
{
    printf("By limit: %.9Lf\n", Euler_number_by_limit(epsilon));
    printf("By series: %.9Lf\n", seriesEulerNumber(epsilon));
    printf("By 'math.h': %.9Lf\n", expl(1));
}


#endif