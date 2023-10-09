#ifndef FUNCTION_H
#define FUNCTION_H

#define MIN_EPSILON 1e-15

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

//info print
void print_help() {
    printf("Choose an action:\n");
    printf("-q: Solving a quadratic equation for all possible unique permutations of the values of the coefficients at powers of the variable.\n");
    printf("-m: Check if the first number is a multiple of the second.\n");
    printf("-t: Сheck whether the remaining three parameters can be the lengths of the sides of a right triangle.\n");
    printf("-help: Display this help message.\n\n");
    printf("-info: Display info message.\n\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_3\n\n");
}
//-----------------



//<-t>
enum right_triangle_status_codes
{
    right_triangle,
    invalid_input_for_triangle,
    not_triangle
};

enum right_triangle_status_codes is_right_triangle(long double epsilon, long double a, long double b, long double c)
{
    if(a < 0 || b < 0 || c < 0 || 
            fabsl(a) < epsilon || fabsl(b) < epsilon || fabsl(c) < epsilon)
    {
        return invalid_input_for_triangle;
    }
    else if (((fabsl(a * a + b * b - c * c) <= epsilon) || (fabsl(c * c + b * b - a * a) <= epsilon) || (fabsl(a * a + c * c - b * b) <= epsilon)))
    {
        return right_triangle;
    }
    return not_triangle;


}

void print_triangle_solution(enum right_triangle_status_codes check, long double a, long double b, long double c)
{
    switch (check)
    {
        case invalid_input_for_triangle:
            printf("You entered incorrect data to check that the triangle is right-angled\n");
            break;
        case not_triangle:
            printf("A triangle with sides %Lf %Lf %Lf is not right\n", a, b, c);
            break;
        case right_triangle:
            printf("A triangle with sides %Lf %Lf %Lf is right\n", a, b, c);
            break;
    }
}
//--------------------------------


//<-m>
enum multiplicity_status_codes{
    multiple,
    not_multiple,
    invalid_input_numbers
};

enum multiplicity_status_codes multiplicity_of_two_numbers(long double first_number, long double second_number){
    if (first_number < 0 || second_number <= 0 || first_number != (long long)first_number || second_number != (long long)second_number)
    {
        return invalid_input_numbers;
    }
    if((long long)first_number % (long long)second_number == 0){
        return multiple;
    }
    return not_multiple;
}


//<-q>
enum solution_status_codes
{
    negative_descrimenant,
    good_descrimenant
};


enum solution_status_codes solution_equation(long double epsilon, long double a, long double b, long double c, long double * x1, long double * x2)
{
    long double descrimenant = b * b - 4.0L * a * c;
    
    if (descrimenant < epsilon)
    {
        return negative_descrimenant;
    }
    *x1 = (-b + sqrtl(descrimenant)) / (2.0L * a);
    *x2 = (-b - sqrtl(descrimenant)) / (2.0L * a);
    return good_descrimenant;
}

void print_solution_the_equation (enum solution_status_codes check, long double a, long double b, long double c, long double x1, long double x2)
{
    switch (check)
    {
        case negative_descrimenant:
            printf("(%Lf)x^2 + (%Lf)x + (%Lf) = 0\nThis equation has no solution in real form\n", a, b, c);
            break;
        case good_descrimenant:
            printf("The roots of a quadratic equation   (%Lf)x^2 + (%Lf)x + (%Lf) = 0 are:\n x1 = %Lf, x2 = %Lf\n", a, b, c, x1, x2);
            break;
    }
}




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
            long double * parameters, char* flag)
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
    else if (argc == 4 || argc == 6)
    {
        for (; i < argc - 1; i++)
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
        if (argc == 6)
        {
            switch (check_epsilon(parameters[0]))
            {
            case good_epsilon:
                break;
            case big_epsilon:
                printf("Epsilon is very large. Range: (1e-15 < x < 1)\n");
                return invalid_input;
            case very_small_epsilon:
                printf("Epsilon is very small. Range: (1e-15 < x < 1)\n");
                return invalid_input;
            }
        }
        switch(flag_checker(argv[i]))
        {
            case invalid_flag:
                return invalid_input_flag;
            case undefined_flag:
                return undefined_input_flag;
            case true_flag:
                *flag = argv[i][1];
                return true_input;
        }
         
    }
    return invalid_input;
}
//--------------------------




void use_flag(char flag, long double * parameters, int argc)
{
    long double epsilon = parameters[0];
    long double a = parameters[1], b = parameters[2], c = parameters[3], x1, x2;
    switch (flag) 
        {
        case 'q':
            if (fabsl(a - b) < epsilon && fabsl(a - c) < epsilon)
            {
                print_solution_the_equation(solution_equation(epsilon, a, a, a, &x1, &x2), a, a, a, x1, x2);
            }
            else if ((fabsl(a - b) < epsilon && fabsl(a - c) >= epsilon) || 
                    (fabsl(a - c) < epsilon && fabsl(c - b) >= epsilon) || 
                    (fabsl(c - b) < epsilon && fabsl(a - c) >= epsilon))
            {
                print_solution_the_equation(solution_equation(epsilon, a, b, c, &x1, &x2), a, b, c, x1, x2);
                print_solution_the_equation(solution_equation(epsilon, b, c, a, &x1, &x2), b, c, a, x1, x2);
                print_solution_the_equation(solution_equation(epsilon, c, a, b, &x1, &x2), c, a, b, x1, x2);
            }
            else
            {
                print_solution_the_equation(solution_equation(epsilon, a, b, c, &x1, &x2), a, b, c, x1, x2);
                print_solution_the_equation(solution_equation(epsilon, b, a, c, &x1, &x2), b, a, c, x1, x2);
                print_solution_the_equation(solution_equation(epsilon, b, c, a, &x1, &x2), b, c, a, x1, x2);
                print_solution_the_equation(solution_equation(epsilon, a, c, b, &x1, &x2), a, c, b, x1, x2);
                print_solution_the_equation(solution_equation(epsilon, c, a, b, &x1, &x2), c, a, b, x1, x2);
                print_solution_the_equation(solution_equation(epsilon, c, b, a, &x1, &x2), c, b, a, x1, x2);
            }
        break;
        case 'm':
            a = parameters[0];
            b = parameters[1];
            switch (multiplicity_of_two_numbers(a, b))
            {
            case multiple:
                printf("The number %lld is a multiple of %lld\n", (long long)a, (long long)b);
                break;
            case not_multiple:
                printf("The number %lld is not a multiple of %lld\n", (long long)a, (long long)b);
                break;
            case invalid_input_numbers:
                printf("Numbers must be non-negative and integers\n");
                break;
            }
            break;
        case 't':
            print_triangle_solution(is_right_triangle(epsilon, a, b, c), a, b, c);
            break;
    }
}
//---------------






#endif