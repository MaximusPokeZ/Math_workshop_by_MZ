#ifndef MATRIX_H
#define MATRIX_H

#define RANGE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>



//digit_check
enum digit_check_status_codes
{
    overflow,
    underflow,
    digit,
    not_digit,
};

enum natural_check_stutus_codes 
{
    natural,
    not_natural
};

enum natural_check_stutus_codes is_natural (const long long value)
{
    if (value >= 1)
    {
        return natural;
    }
    return not_natural;
}

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

enum digit_check_status_codes is_digit(const char *str, long long *value_from_string) {
    char *endptr;
    *value_from_string = strtoll(str, &endptr, 10); 
    if (*str != '\0' && *endptr == '\0')
    {
        return overflow_underflow_normal(*value_from_string);
    }
    value_from_string = NULL;
    return not_digit;
}
//-----------------------------------------------------


// <-f>
enum factorial_status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter
};

enum factorial_status_codes factorial(unsigned int number, unsigned long * result)
{
    enum factorial_status_codes recursive_status_code; 
    if (number > 22) 
    { 
        return fsc_invalid_parameter; 
    } 

    if (number == 0 || number == 1)
    {
        *result = 1;
        return fsc_ok;
    }

    recursive_status_code = factorial(number - 1, result);

    if (recursive_status_code == fsc_ok)
    {
                
        if (*result > ULONG_MAX / number) {
            return fsc_overflow;
        }
        *result *= number;
    }

    return recursive_status_code;
}

void print_factorial (enum factorial_status_codes status, const long long number, unsigned long result)
{
    switch(status)
    {
        case fsc_ok: 
            printf("%lld! = %lu\n", number, result); 
            break; 
        case fsc_overflow: 
            printf("Overflow detected!"); 
            break; 
        case fsc_invalid_parameter: 
            printf("Invalid parameter detected! (0<x<20)"); 
            break; 
    }
}

//---------------------------------------------




// <-a>
enum digit_check_status_codes sum_of_natural_numbers(long long number, long long * result)
{
    switch (is_natural(number))
    {
        case natural:
            *result = number * (number + 1) / 2;
            if (*result < number) return overflow;
            return overflow_underflow_normal(*result);
        case not_natural:
            return not_digit;
    }
}

void print_sum_of_natural_numbers (enum digit_check_status_codes status, long long sum, long long value)
{
    switch (status)
    {
    case overflow:
        printf("Overflow sum\n");
        break;
    case underflow:
        printf("Underflow sum\n");
        break;
    case not_digit:
        printf("Not natural input\n");
        break;
    case digit:
        printf("Sum of natural numbers from 1 to %lld is %lld\n", value, sum);
        break; 
    }
}


//-----------------------



bool is_negative(char* number)
{
    if (number[0] == '-')
    {
        return true;
    }
    return false;
}

bool is_negative_null(char ** str_number, long long number)
{
    if (is_negative(*str_number) && number == 0)
    {
        *str_number = "0";
        return true;
    }
    return false;
}



//divide digits <-s>
void print_digits(const unsigned char* array)
{
    printf("Divide a number into digits: ");
    while (*array != '\0')
    {
        printf("%c ", *array);
        array++;
    }
    printf("\n");
}


void divide_to_digits (char* number, unsigned char *** array)
{
    **array = (unsigned char * )malloc(sizeof(unsigned char) * RANGE);
    int i = 0;
    if (is_negative(number))
    {
        (**array)[i] = *number;
        number++;
        i++;
    }
    if (*(number + 1) == '\0')
    {
        (**array)[i] = *number;
        (**array)[++i] = '\0';
        return;
    }
    while (*number == '0') 
    {
        number++;
    }
    while (*number != '\0')
    {
        (**array)[i] = *number;
        number++;
        i++;
    }
    (**array)[i] = '\0';
}
//---------------------------------------------------------------


// Print prime <-p>
enum prime_status_code
{
    prime,
    not_prime
};


enum prime_status_code is_prime(const long long * number) {
    if (*number <= 1) {
        return not_prime;
    }
    for (int divisor = 2; divisor * divisor <= *number; divisor++) {
        if (*number % divisor == 0) {
            return not_prime;
        }
    }
    return prime;
}

void print_prime(enum prime_status_code status, const long long number)
{
    switch(status) 
    {
        case prime:
            printf("%lld is prime number\n", number);
            break;
        case not_prime:
            printf("%lld is composite number\n", number);
            break;
    } 
}
//------------------------------------------------------



// Print multiples <-h>
enum multiples_status_codes
{
    has_multiples,
    not_multiples
};

enum multiples_status_codes find_multiples(int divisor, unsigned char *** array_of_multiples)
{
    bool flag_has_multiples = false;
    **array_of_multiples = (unsigned char *)malloc(sizeof(unsigned char) * (RANGE + 1));
    if (**array_of_multiples == NULL || divisor <= 0) {
        return not_multiples;
    }

    for (int dividend = divisor; dividend <= RANGE; dividend += divisor)
    {
        flag_has_multiples = true;
        (**array_of_multiples)[dividend] = 1;
    }
    if (!flag_has_multiples)
    {
        return not_multiples;
    }
    return has_multiples;
}

void print_multiples(enum multiples_status_codes status, const int number, const unsigned char * array)
{
    switch (status)
    {
        case not_multiples:
            printf("The number %d has no multiples\n", number);
            break; 
        case has_multiples:
            printf("Numbers that are multiples of %d: ", number);
            for (int i = number; i <= RANGE; i += number)
            {
                printf("%d ", i);
            }
            printf("\n");
            break;
    }
    
}
//-------------------------------------------------



//<-e>
unsigned long power(int base, int degree) {
    unsigned long result = 1;
    for (int i = 0; i < degree; i++) {
        result *= base;
    }
    return result;
}

void table_of_degrees(const int to_this_degree)
{
    if (to_this_degree >= 1 && to_this_degree <= 10)
    {
        printf("Table of powers for bases from 1 to 10, degree range: 1 to %d", to_this_degree);
        for (int base = 1; base <= 10; base++) {
            printf("\n\nFor base %d:\t", base);
            for (int degree = 1; degree <= to_this_degree; degree++)
            {
                printf("%d^%d = %lu; ", base, degree, power(base, degree));
            }
        }
        printf("\n");
    }
    else
    {
        printf("The number entered must be no more than 10 and not negative\n");
        return;
    }
}


//info print
void print_help() {
    printf("Choose an action:\n");
    printf("-h: Print natural numbers within 100 that are multiples of the specified number.\n");
    printf("-p: Check if a number is prime or composite.\n");
    printf("-s: Divide the number into individual digits and print them.\n");
    printf("-a: Calculate the sum of natural numbers from 1 to the specified number.\n");
    printf("-f: Calculate the factorial of the specified number.\n");
    printf("-help: Display this help message.\n\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_1\n\n");
}
//-----------------



//Checkers
enum flag_check_status_codes
{
    invalid_flag,
    undefined_flag,
    true_flag
};

enum flag_check_status_codes flag_checker(char* flag)
{
    if (flag[0] != '-')
    {
        return invalid_flag;
    }
    else if (flag[2] != '\0' || 
            (flag[1] != 'h' && flag[1] != 'p' && 
            flag[1] != 's' && flag[1] != 'e' &&
            flag[1] != 'a' && flag[1] != 'f'))
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

enum input_check_status_codes Input_checker(int argc, char* argv[], long long * number, char** str_number, char* flag)
{
    if (argc != 3)
    {
        if (argc == 2 && !(strcmp(argv[1], "-help"))) {
            return info_input_help;
        }
        else if (argc == 2 && !(strcmp(argv[1], "-info"))) {
            return info_input;
        }
        return invalid_input;
    }
    switch (is_digit(argv[1], number))
    {
        case digit: 
            *str_number = argv[1]; 
            switch(flag_checker(argv[2]))
            {
                case invalid_flag:
                    return invalid_input_flag;
                case undefined_flag:
                    return undefined_input_flag;
                case true_flag:
                    *flag = argv[2][1];
                    return true_input;
            }
        case not_digit:
            return invalid_input;
        case overflow:
            return overflow_input;
        case underflow:
            return underflow_intput;
    }
    return invalid_input;
} 

void use_flag(long long * number, char ** str_number, char flag, unsigned char ** array_for, long long * result)
{
    switch (flag) 
        {
        case 'h':
            print_multiples(find_multiples(*number, &array_for), *number, *array_for);
            break;
        case 'p':
            print_prime(is_prime(number), *number);
        break;
        case 's':
            divide_to_digits(*str_number, &array_for);
            print_digits(*array_for);
            break;
        case 'a':
            print_sum_of_natural_numbers(sum_of_natural_numbers(*number, result), *result, *number);
            break;
        case 'f':
            print_factorial(factorial(*number, (unsigned long * )result), *number, *result);
            break;
        case 'e':
            table_of_degrees(*((int *)number));
            break;
        default:
            printf("Invalid flag: %c\n", flag);
            break;
    }
}
//---------------






#endif