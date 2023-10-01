#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>


#define MIN_EPSILON 1e-15


//info print
void print_help(const char * programm) {

    printf("Use: %s <epsilon> or use %s -help to list all functions\n\n", programm, programm);
    printf("-help: Display this help message.\n\n");
    printf("-info: Display info message.\n\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_5\n\n");
}
//-----------------



//Input & epsilon check
//epsilon check
enum epsilon_status_codes
{
    good_epsilon,
    big_epsilon,
    very_small_epsilon,
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
//-----------------------



//digit_check
enum digit_check_status_codes
{
    overflow,
    underflow,
    digit,
    not_digit,
};

enum digit_check_status_codes overflow_underflow_normal (const long long value)
{
    if (value >= LLONG_MAX)
    {
        return overflow;
    }
    else if (value <= LLONG_MIN)
    {
        return underflow;
    }
    return digit;
}

enum digit_check_status_codes is_digit(const char *str, long double *value_from_string) {
    char *endptr;
    *value_from_string = strtold(str, &endptr); 
    if (*str != '\0' && *endptr == '\0')
    {
        return overflow_underflow_normal(*value_from_string);
    }
    value_from_string = NULL;
    return not_digit;
}
//-----------------------------------------------------



//Checkers
enum flag_check_status_codes
{
    invalid_flag,
    undefined_flag,
    true_flag
};

enum flag_check_status_codes flag_checker(char* flag)
{
    if (flag[0] != '-' && flag[0] != '/')
    {
        return invalid_flag;
    }
    else if (flag[2] != '\0' || 
            (flag[1] != 'q' && flag[1] != 'm' && flag[1] != 't'))
    {
        return undefined_flag;
    }
    return true_flag;
}

enum input_check_status_codes
{
    overflow_input,
    underflow_intput,
    invalid_input, 
    invalid_input_flag, 
    undefined_input_flag,
    info_input_help,
    info_input,
    true_input
};

enum input_check_status_codes Input_checker(int argc, char* argv[], 
            long double * parameters)
{
    int i = 1;
    if (argc == 2) {
        if (!(strcmp(argv[1], "-help"))) {
            return info_input_help;
        }
        else if (!(strcmp(argv[1], "-info"))) {
            return info_input;
        }
        return invalid_input;
    }
    else if (argc == 3)
    {
        for (; i < argc; i++)
        {
            switch (is_digit(argv[i], &parameters[i - 1]))
            {
            case digit:
                break;
            case not_digit:
                return invalid_input;
            case overflow:
                return overflow_input;
            case underflow:
                return underflow_intput;
            }
        }
        switch (check_epsilon(parameters[1]))
            {
            case good_epsilon:
                return true_input;
            case big_epsilon:
                printf("Epsilon is very large. Range: (1e-15 < x < 1)\n");
                return invalid_input;
            case very_small_epsilon:
                printf("Epsilon is very small. Range: (1e-15 < x < 1)\n");
                return invalid_input;
            }
    }
    return invalid_input;
}
//-------------------------------------

void print_sum_ (const long double result, const char name)
{
    printf("The value of the sum %c: %.9Lf\n", name, result);
}


//a
long double sum_a (const long double x, const long double epsilon)
{   
    long double prev_term = 1.0L;
    long double term = x;
    long double sum = 1.0L + x;
    long long n = 2;
    do
    {
        prev_term = term;
        term = (term * x) / n;
        sum += term;
        n++;
    } while (fabsl(prev_term - term) > epsilon);
    return sum;
}
//--------------------------------


//b
long double sum_b (const long double x, const long double epsilon)
{   
    long double prev_term = 0.0L;
    long double term = 1.0L;
    long double sum = 1.0L;
    long long n = 1;
    do
    {
        prev_term = term;
        term = (-1 * prev_term * x * x) / ((2 * n) * (2 * n - 1));
        sum += term;
        n++;
    } while (fabsl(prev_term - term) > epsilon);
    return sum;
}
//--------------------------------


#endif