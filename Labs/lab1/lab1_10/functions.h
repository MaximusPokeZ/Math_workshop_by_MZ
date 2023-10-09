#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 33

enum start_status_codes
{
    good_enter_radix,
    not_good_radix
};

enum start_status_codes check_start_enter(int radix)
{
    if (radix < 2 || radix > 36)
    {
        return not_good_radix;
    }
    return good_enter_radix;
}


int char_to_num(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return c - 'A' + 10;
    }
    return -1; 
}

enum correct_input_status_codes
{
    correct,
    not_correct
};

enum transfer_from_str
{
    ok_transfer,
    invalid_character
};

enum transfer_from_str ss_to_base_10(char* str, int base, long long * result) {
    int len = strlen(str);
    int i = len - 1;
    long long multiplier = 1;
    for (; i > 0; i--) {
        int digit = char_to_num(str[i]);
        if (digit == -1 || digit >= base) {
            return invalid_character;
        }
        *result = *result + digit * multiplier;
        multiplier *= base;
    }
    if (str[i] == '-')
    {
        *result = -(*result);
    }
    else
    {
        int digit = char_to_num(str[i]);
        if (digit == -1 || digit >= base) {
            return invalid_character;
        }
        *result = *result + digit * multiplier;
        multiplier *= base;
    }
    return ok_transfer;
}

void transfer_from_int(char *str, long long number, int base)
{
    int r, sign = 1, len = 0;
    if (number < 0)
    {
        sign = -1;
        number = -number;
        len++;
    }
    if (number != 0)
    {
        len += logl(number) / logl(base);
    }
    else
    {
        str[0] = '0';
    }


    char *ptr = str + len + 1;
    *ptr = '\0';
    ptr--;
    while (number > 0)
    {
        r = number % base;
        *ptr = (r > 9) ? r - 10 + 'A' : r + '0';
        ptr--;
        number /= base;
    }
    if (sign == -1)
    {
        *ptr = '-';
    }
}

void print_to_radix(char *str, int base)
{
    printf("Your number in the %d number system: %s\n", base, str);
}

#endif