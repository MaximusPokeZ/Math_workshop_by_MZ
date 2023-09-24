#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


bool is_digit(const char *str, unsigned long *value_from_string) {
    char *endptr;
    *value_from_string = strtoul(str, &endptr, 10); 
    if (*str != '\0' && *endptr == '\0')
    {
        return true;
    }
    value_from_string = NULL;
    return false;
}

bool is_negative(char* number)
{
    if (number[0] == '-')
    {
        return true;
    }
    return false;
}

long long power(int base, int degree) {
    long long result = 1;
    for (int i = 0; i < degree; i++) {
        result *= base;
    }
    return result;
}

void print_multiples(int divisor)
{
    bool flag_has_multiples = false;
    for (int dividend = 1; dividend <= 100; dividend++)
    {
        if (dividend % divisor == 0)
        {
            printf("%d ", dividend);
            flag_has_multiples = true;
        }
    }
    if (!flag_has_multiples)
    {
        printf("No multiples found\n");
    }
    printf("\n");
}

bool is_prime(int number) {
    if (number <= 1) {
        return false;
    }
    for (int divisor = 2; divisor * divisor <= number; divisor++) {
        if (number % divisor == 0) {
            return false;
        }
    }
    return true;
}

void print_digits (char* number)
{
    printf("Divide a number into digits: ");

    if (is_negative(number))
    {
        printf("%c ", *number);
        number++;
    }
    if (*(number + 1) == '\0')
    {
        printf("%c\n", *number);
        return;
    }
    while (*number == '0') 
    {
        number++;
    }
    while (*number != '\0')
    {
        printf("%c ", *number);
        number++;
    }
    printf("\n");
}

int sum_of_natural_numbers(int number)
{
    int sum = number * (number + 1) / 2;
    return sum;
}

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

void table_of_degrees(char * number)
{
    int to_this_degree = atoi(number);
    if (to_this_degree >= 1 && to_this_degree <= 10)
    {
        printf("Table of powers for bases from 1 to 10, degree range: 1 to %d", to_this_degree);
        for (int base = 1; base <= 10; base++) {
            printf("\n\nFor base %d:\t", base);
            for (int degree = 1; degree <= to_this_degree; degree++)
            {
                printf("%d^%d = %lld; ", base, degree, power(base, degree));
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
    else if (flag[1] != 'h' || flag[1] != 'p' || 
                        flag[1] != 's' || flag[1] != 'e' ||
                                flag[1] != 'a' || flag[1] != 'f')
    {
        return undefined_flag;
    }
    return true_flag;
}

enum input_check_status_codes
{
    invalid_input, 
    invalid_input_flag, 
    undefined_input_flag,
    info_input_help,
    info_input,
    true_input
};

enum input_check_status_codes Input_checker(int argc, char* argv[], unsigned long * number)
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
    if (is_digit(argv[1], number))
    {
        switch(flag_checker(argv[2]))
        {
            case invalid_flag:
                return invalid_input_flag;
            case undefined_flag:
                return undefined_input_flag;
            case true_flag:
                return true_input;
        }
    }
    return invalid_input;
} 



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

int main(int argc, char* argv[])
{

    unsigned long number;

    switch (Input_checker(argc, argv, &number))
    {
    case invalid_input:
        printf("\nInvalid input\n");
        printf("Use: %s <number> <flag> (use %s -help to list all functions)\n\n", argv[0], argv[0]);
        return 1;
    case info_input:
        print_info();
        break;
    case info_input_help:
        print_help();
        break;
    case invalid_input_flag:
        printf("Invalid flag\n");
        return 1;
    case undefined_input_flag:
        printf("Undefined flag\n");
        return 1;
    case true_input:
        printf("True input\n");
        break;
    }
    

    // char* str_number = argv[1];
    // char flag = argv[2][1];
    // unsigned long result;

    //printf("%lu\n", number);


//     switch (flag) 
//     {
//     case 'h':
//         print_multiples(atoi(number));
//         break;
//     case 'p':
//         if (is_prime(atoi(number))) 
//         {
//             printf("%s is prime number\n", number);
//         } else 
//         {
//             printf("%s is composite number\n", number);
//         }
//         break;
//     case 's':
//         print_digits(number);
//         break;
//     case 'a':
//         printf("Sum of natural numbers from 1 to %s is %d\n", number, sum_of_natural_numbers(atoi(number)));
//         break;
//     case 'f':
//         switch(factorial(atoi(number), &result))
//         {
//             case fsc_ok: 
//                 printf("%s! = %lu\n", number, result); 
//                 break; 
//             case fsc_overflow: 
//                 printf("Overflow detected!!1!1"); 
//                 break; 
//             case fsc_invalid_parameter: 
//                 printf("Invalid parameter detected!!1!1"); 
//                 break; 
//         }
//         break;
//     case 'e':
//         table_of_degrees(number);
//         break;
//     default:
//         printf("Invalid flag: %c\n", flag);
//         break;
// }

return 0;


}