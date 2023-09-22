#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


bool is_digit(char* number)
{
    if (((number[0] >= '0') && (number[0] <= '9')) || (number[0] == '-' && ((number[1] >= '0') && (number[1] <= '9'))))
    {
        return true;
    }
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

int factorial(int number)
{
    return (number < 2) ? 1 : number * factorial(number - 1);
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
    printf("Laboratory work #1_1\n\n");
}

int main(int argc, char* argv[])
{

    if (argc != 3)
    {
        if (argc == 2 && !(strcmp(argv[1], "-help"))) {
            print_help();
            return 0;
        }
        else if (argc == 2 && !(strcmp(argv[1], "-info"))) {
            print_info();
            return 0;
        }
        printf("\nInvalid input\n");
        printf("Use: %s <number> <flag> (use %s -help to list all functions)\n\n", argv[0], argv[0]);
        return 1;
    }

    char* number = argv[1];
    char flag = argv[2][1];

    if (is_digit(number))
    {
        if (is_negative(number) && number[1] == '0' && number[2] == '\0')
        {
            number = "0";
        }
    }
    else
    {
        printf("Incorrect number entry\n");
        return 1;
    }

    switch (flag) 
    {
    case 'h':
        print_multiples(atoi(number));
        break;
    case 'p':
        if (is_prime(atoi(number))) 
        {
            printf("%s is prime number\n", number);
        } else 
        {
            printf("%s is composite number\n", number);
        }
        break;
    case 's':
        print_digits(number);
        break;
    case 'a':
        printf("Sum of natural numbers from 1 to %s is %d\n", number, sum_of_natural_numbers(atoi(number)));
        break;
    case 'f':
        if (*number != '-') {
            printf("Factorial of %s is %d\n", number, factorial(atoi(number)));
        } else {
            printf("Factorial for positive numbers only\n");
        }
        break;
    case 'e':
        table_of_degrees(number);
        break;
    default:
        printf("Invalid flag: %c\n", flag);
        break;
}

return 0;


}