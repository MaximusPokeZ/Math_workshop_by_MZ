#ifndef MATRIX_H
#define MATRIX_H

#define RANGE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


// Print prime <-p>
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

// Print multiples <-h>
void print_multiples(int number, unsigned char * array)
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