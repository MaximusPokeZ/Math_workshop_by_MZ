#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>


#define MIN_EPSILON 1e-9
#define CONST_EULER_GAMMA 0.577215665

long double precalculation_for_gamma;



const long long MAX_N = 1e5;
 
long long sieve_of_eratoshen[MAX_N + 1];
 
void sieve()
{
    for (long long i = 2; i * i <= MAX_N; ++i)
    {
        if (sieve_of_eratoshen[i] > 0)
        {
            continue;
        }
        for (long long j = 2;  i * j <= MAX_N; ++j)
        {
            sieve_of_eratoshen[i * j] = 1;
        }
    }
}



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



// Newton's method for solving the equation f(x) = 0
long double newton_Method(long double a, long double b, long double epsilon, long double (*function) (long double), long double (*dfunction) (long double)) {
    long double x_0 = (a + b) / 2;
    long double x_1 = x_0 - function(x_0) / dfunction(x_0);

    while (fabsl(x_0 - x_1) > epsilon) {
        x_0 = x_1;
        x_1 = x_0 - function(x_0) / dfunction(x_0);
    }
    return x_1;
}

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

long double series_Euler_Number(long double epsilon)
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


long double function_lnX_1(long double x) {
    return log(x) - 1;
}

// Derivative of the function ln(x) - 1
long double dfunction_ln(long double x) {
    return 1.0 / x;
}


void print_E (long double epsilon)
{
    printf("By limit: %.9Lf\n", Euler_number_by_limit(epsilon));
    printf("By series: %.9Lf\n", series_Euler_Number(epsilon));
    printf("By ln(x) = 1 && Newton's method: %.9Lf\n", newton_Method(1.0L, 3.0L, epsilon, &function_lnX_1, &dfunction_ln));
    printf("By 'math.h' %.9Lf\n\n", expl(1));
}
//--------------------------------

void multiplication_for_n_plus_one (long double * new_value, int n)
{
    *new_value = *new_value * ((4.0L * n * (n + 1)) / ((2.0L * n + 1) * (2.0L * n + 1)));
}

//for Pi
long double Pi_number_by_limit (long double epsilon)
{
    
    long double pi = 1.0L, factorial = 4.0L;
    int n = 1;
    while (fabsl(pi - factorial) > epsilon)
    {
        pi = factorial;
        multiplication_for_n_plus_one(&factorial, n);
        n++;
    }
    return pi;   
}

long double series_pi_Number(long double epsilon)
{
    long double pi = 1.0L;
    long double term = 1.0L;
    long double sign = -1.0L;
    int n = 1;

    while (fabsl(term) > epsilon)
    {
        n++;
        term = sign / (2 * n - 1);
        pi += term;
        sign = -sign;
    }
    pi *= 4;
    return pi;
}


long double function_cosx_1(long double x) {
    return cos(x) + 1;
}

// Derivative of the function cos(x) + 1
long double dfunction_cos(long double x) {
    return -sin(x);
}


void print_pi (long double epsilon)
{
    printf("By limit: %.9Lf\n", Pi_number_by_limit(epsilon));
    printf("By series: %.9Lf\n", series_pi_Number(epsilon));
    printf("By cos(x) = 1 && Newton's method: %.9Lf\n", newton_Method(1.0L, 5.0L, epsilon, &function_cosx_1, &dfunction_cos));
    printf("By 'math.h' %.9f\n\n", M_PI);
}
//--------------------------------


//for ln(2)
long double ln_number_by_limit (long double epsilon)
{
    
    long double ln2 = 0.0L, prev_ln2 = 0.0L;
    int n = 1;
    do
    {
        prev_ln2 = ln2;
        ln2 = n * (pow(2, 1.0L / n) - 1);
        n++;
    }
    while (fabsl(ln2 - prev_ln2) > epsilon);
    return ln2;   
}

long double series_ln_Number(long double epsilon)
{
    long double ln = 0.0L;
    long double term = 0.0L;
    long double sign = 1.0L;
    int n = 1;
    do
    {
        term = sign / n;
        ln += term;
        n++;
        sign = -sign;
    }
    while (fabsl(term) > epsilon);
    return ln;
}


long double function_ex_2(long double x) {
    return expl(x) - 2;
}

// Derivative of the function cos(x) + 1
long double dfunction_ex(long double x) {
    return expl(x);
}


void print_ln (long double epsilon)
{
    printf("By limit: %.9Lf\n", ln_number_by_limit(epsilon));
    printf("By series: %.9Lf\n", series_ln_Number(epsilon));
    printf("By e^x = 2 && Newton's method: %.9Lf\n", newton_Method(0.0L, 1.0L, epsilon, &function_ex_2, &dfunction_ex));
    printf("By 'math.h' %.9f\n\n", log(2));
}
//--------------------------------


//for sqrt(2)
long double sqrt_number_by_limit (long double epsilon)
{
    
    long double xn = -0.5L, prev_xn = 0.0L;
    do
    {
        prev_xn = xn;
        xn = xn - (xn * xn) * 0.5L + 1;
    }
    while (fabsl(prev_xn - xn) > epsilon);
    return xn;   
}

long double Product_sqrt_Number(long double epsilon)
{
    long double result = 1.0L;
    long double term = 2.0L;
    long double term_degree = 0.5L;
    do
    {
        term_degree = term_degree * 0.5L;
        term = pow(2.0L, term_degree);
        result *= term;
    }
    while (fabsl(term_degree) > epsilon);
    return result;
}


long double function_x2_2(long double x) {
    return x * x - 2;
}

// Derivative of the function cos(x) + 1
long double dfunction_x2(long double x) {
    return 2.0L * x;
}


void print_sqrt (long double epsilon)
{
    printf("By limit: %.9Lf\n", sqrt_number_by_limit(epsilon));
    printf("By product: %.9Lf\n", Product_sqrt_Number(epsilon));
    printf("By x^2 = 2 && Newton's method: %.9Lf\n", newton_Method(1.0L, 2.0L, epsilon, &function_x2_2, &dfunction_x2));
    printf("By 'math.h' %.9f\n\n", sqrt(2));
}
//--------------------------------


//for γ gamma
long double gamma_number_by_limit (long double epsilon)
{
    
    long double gamma = 0.0L, term = 0.0L;
    int k = 1;
    do
    {
        term = 1.0L / k;
        gamma += term; 
        k++;
    }
    while (fabsl(term) > epsilon);
    return gamma - log(k);   
}

long double series_gamma_Number(long double epsilon)
{
    long double gamma = 0.0L;
    long double term = 0.0L, prev_term = 0.0L;
    int k = 2;
    do
    {
        prev_term = term;
        term = (1.0L / bpow(floorl(sqrt(k)), 2)) - (1.0L / k);
        gamma += term;
        k++;
    }
    while (fabsl(term - prev_term) > epsilon);
    gamma = gamma - ((M_PI * M_PI) / 6.0L);
    return gamma;
}

long long Get_next_prime (long long p)
{
    p++;
    while (sieve_of_eratoshen[p] != 0)
    {
        p++;
    } 
    return p;
}

void precalculation_product (long double epsilon)
{
    precalculation_for_gamma = 1.0L; 
    long double term = 1.0L, prev_term = 0.0L;
    long long p = 1, t; 
    do
    {
        p = Get_next_prime(p);
        t = p;
        prev_term = term;
        term = ((long double)p - 1) / (long double)p;
        precalculation_for_gamma *= term;
        t++;
    } while (fabsl(term - prev_term) > epsilon);
    precalculation_for_gamma = precalculation_for_gamma * log(t);
}


long double function_e_gamma(long double x) {
    return expl(-x) - precalculation_for_gamma;
}

// Derivative of the function cos(x) + 1
long double dfunction_e_gamma(long double x) {
    return -expl(-x);
}


void print_gamma (long double epsilon)
{
    printf("By limit: %.9Lf\n", gamma_number_by_limit(epsilon));
    printf("By series: %.9Lf\n", series_gamma_Number(epsilon));
    sieve();
    precalculation_product(epsilon);
    printf("By e^-x = lim(ln(t)  П (p - 1) / p) && Newton's method: %.9Lf\n", newton_Method(0.0L, 1.0L, epsilon, &function_e_gamma, &dfunction_e_gamma));
    printf("By 'math.h' %.9f\n\n", CONST_EULER_GAMMA);
}
//--------------------------------


#endif