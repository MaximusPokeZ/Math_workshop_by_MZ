#ifndef MATRIX_H
#define MATRIX_H

#define RANGE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

// <-a>
long long sum_of_natural_numbers(long long number)
{
    long long sum = number * (number + 1) / 2;
    return sum;
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
//------------------------------------------------------

// Print multiples <-h>
void print_multiples(const int number, const unsigned char * array)
{
    printf("Numbers that are multiples of %d:\n", number);
    for (int i = number; i <= RANGE; i += number)
    {
        printf("%d ", i);
    }
    printf("\n");
}

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
//-------------------------------------------------

#endif