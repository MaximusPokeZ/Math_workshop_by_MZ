#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>


#define MIN_EPSILON 1e-9


//info print
void print_help(const char * programm) {

    printf("Use: %s <epsilon> or use %s -help to list all functions\n\n", programm, programm);
    printf("-help: Display this help message.\n\n");
    printf("-info: Display info message.\n\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_6\n\n");
}
//-----------------

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



//Input & epsilon check
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

enum input_check_status_codes Input_checker(int argc, char* argv[], long double * epsilon)
{
    if (argc == 2)
    {
        switch (is_digit(argv[1], epsilon))
        {
            case good_epsilon: 
                return good_input;
            case big_epsilon:
                printf("Epsilon is very large. Range: (1e-15 < x < 1)\n");
                return invalid_input;
            case very_small_epsilon:
                printf("Epsilon is very small. Range: (1e-15 < x < 1)\n");
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


void print_integral_sum (const long double result, const char name)
{
    printf("Solution of integral #%c : %Lf\n", name, result);
}

long double func_1(long double x, long double epsilon) {
    if (x == 0.0L) {
        return log(1 + x) / (x + epsilon);
    }
    return log(1 + x) / x;
}

long double func_2(long double x, long double epsilon) {
    if (x == 0.0L) {
        return 1.0L;
    }
    return exp(-x * x * 0.5);
}

long double func_3(long double x, long double epsilon) {
    if (x == 1.0L) {
        return -log(1 - x + epsilon);
    }
    return -log(1 - x);
}

long double func_4(long double x, long double epsilon) {
    if (x == 0.0L) {
        return powl(x + epsilon, x + epsilon);
    }
    return powl(x, x);
}



long double simpson_method (long double a, long double b, long double epsilon, long double (*function) (long double, long double)) {
    int n = 2; 
    long double integral = 0.0L;
    long double prev_integral = 0.0L;

    do {
        prev_integral = integral;
        integral = function(a, epsilon) + function(b, epsilon);
        long double h = (b - a) / n;
        for (int i = 1; i < n; i++) {
            long double x = a + i * h;
            long double fx = function(x, epsilon);
            if (i % 2 == 1) {
                integral += 4 * fx;
            } else {
                integral += 2 * fx;
            }
        }

        integral = (integral * h) / 3.0L;
        n *= 2;
    } while (fabsl(integral - prev_integral) > epsilon);

    return integral;
}





//--------------------------------

#endif