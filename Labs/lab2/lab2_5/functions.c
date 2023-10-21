#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#define FOR_TRANSFER 20
#define MAX_RANGE 1e6
#define SIZE_TO_BASE 34

enum DataType 
{
    INT,
    UINT,
    DOUBLE,
    FLOAT
};

enum transfer_to_status_codes
{
    out_of_range,
    memory_allocation_problem,
    ok_transfer
};

enum transfer_to_status_codes int_transfer_to_roman(int num, char **result)
{
    if (num < 0 || num > 3999)
    {
        return out_of_range;
    }
    char *roman_numerals_alf[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    *result = (char *)malloc(sizeof(char) * FOR_TRANSFER);
    if (*result == NULL)
    {
        return memory_allocation_problem;
    }
    if (num == 0)
    {
        strcat(*result, "nihil");
        return ok_transfer;
    }
    for (int i = 0; num > 0; i++)
    {
        while (num >= values[i])
        {
            strcat(*result, roman_numerals_alf[i]);
            num -= values[i];
        }
    }
    return ok_transfer;
}

enum transfer_to_status_codes transfer_cyckendorf(unsigned int num, char **result)
{
    if (num > MAX_RANGE)
    {
        return out_of_range;
    }
    unsigned int *fib = NULL, *fib_ptr = NULL;
    char *res_ptr = NULL;
    unsigned int size = FOR_TRANSFER;
    fib = (unsigned int *)malloc(sizeof(unsigned int) * size);
    if (fib == NULL)
    {
        return memory_allocation_problem;
    }
    *result = (char *)malloc(sizeof(char) * size);
    if (*result == NULL)
    {
        free(fib);
        return memory_allocation_problem;
    }

    fib[0] = 1;
    fib[1] = 1;
    unsigned int fib_len = 2;
    unsigned int current = 1;
    while (current <= num)
    {
        current = fib[fib_len - 1] + fib[fib_len - 2];
        if (current > num)
        {
            break;
        }
        fib[fib_len] = current;
        fib_len++;
        if (fib_len == size)
        {
            size *= 2;
            fib_ptr = (unsigned int *)realloc(fib, size * sizeof(unsigned int));
            if (fib_ptr == NULL)
            {
                free(fib);
                return memory_allocation_problem;
            }
            fib = fib_ptr;
            res_ptr = (char *)realloc(*result, size * sizeof(char));
            if (res_ptr == NULL)
            {
                free(*result);
                return memory_allocation_problem;
            }
            *result = res_ptr;
        }
    }
    (*result)[fib_len] = '1';
    (*result)[fib_len + 1] = '\0';
    for (int j = fib_len - 1; j >= 0; j--)
    {
        if (num >= fib[j])
        {
            (*result)[j] = '1';
            num -= fib[j];
        }
        else
        {
            (*result)[j] = '0';
        }
    }
    free(fib);
    return ok_transfer;
}



enum transfer_to_status_codes int_transfer_to_base(int num, char **result, int base, char flag, int * counter, bool * sign)
{
    if (num > MAX_RANGE)
    {
        return out_of_range;
    }
    *result = (char *)malloc(sizeof(char) * FOR_TRANSFER);
    if (*result == NULL)
    {
        return memory_allocation_problem;
    }
    if (base < 2 || base > 36) base = 10;
    *sign = false;
    if (num < 0) 
    {
        *sign = true;
        num = -num;
    }
    char *ptr = *result + SIZE_TO_BASE - 1 + *sign;
    *counter = 0;
    int r;
    *ptr = 0;
    ptr--;
    while (num > 0) {
        r = num % base;
        if (flag == 'v') *ptr = (r > 9) ? r - 10 + 'A' : r + '0';
        else *ptr = (r > 9) ? r - 10 + 'a' : r + '0';
        ptr--;
        num /= base;
        (*counter)++;
    }
    if (*sign)
    {
        *ptr = '-';
    }
    return ok_transfer;
}

enum transfer_to_status_codes ss_to_base_10(char* str, int base, long long * result, char flag) 
{
    *result = 0;
    int len = strlen(str);
    bool sign = false;
    if (len > 8) return out_of_range;
    if (base < 2 || base > 36) base = 10;
    if (str[0] == '-') sign = true;
    int i = len - 1;
    long long multiplier = 1;
    int digit;
    for (; i >= 0 + sign; i--) 
    {
        char c = str[i];
        if (flag == 'T') digit = (c >= '0' && c <= '9') ? c - '0' : c - 'A' + 10;
        else digit = (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10;
        if (digit >= base) return out_of_range;
        *result = *result + digit * multiplier;
        multiplier *= base;
    }
    if (sign)
    {
        *result = -(*result);
    }
    return ok_transfer;
}

void print_memory_dump(const void* value, enum DataType type, FILE* stream, int* result, char ** string) {
    const unsigned char* bytes = (const unsigned char*)value;
    int size = 0;

    if (type == INT) size = sizeof(int);
    else if (type == UINT) size = sizeof(unsigned int);
    else if (type == FLOAT) size = sizeof(float);
    else if (type == DOUBLE) size = sizeof(double);
    else
    {
        if (stream == NULL) {*result += sprintf(*string, "Unknown data type\n"); *string += strlen("Unknown data type\n");}
        else *result += fprintf(stream, "Unknown data type\n");
        return;
    }

    for (int i = 0; i < size; i++) 
    {
        for (int bit = 7; bit >= 0; bit--) 
        {
            int bit_value = (bytes[i] >> bit) & 1;
            if (stream == NULL) {*result += sprintf(*string, "%d", bit_value); (*string)++;}
            else *result += fprintf(stream, "%d", bit_value);
        }
        if (stream == NULL) {*result += sprintf(*string, " "); (*string)++;}
        else *result += fprintf(stream, " ");
        
    }
}
