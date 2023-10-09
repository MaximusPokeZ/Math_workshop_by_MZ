#ifndef FUNCTION_H
#define FUNCTION_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#define FIXED_SIZE_ARRAY 30
#define MIN_RANGE 10
#define MAX_RANGE 10000

void print_help(char * programm_name) {
    printf("Use %s <range1 a > <range2 b >\n", programm_name);
    printf("-help: Display this help message.\n");
    printf("-info: Display info message.\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_9\n\n");
}

//secondary functions
bool is_number(char *str, long long * number)
{
    char* str_number;
    *number = strtol(str, &str_number, 10);
    if (*str_number == '\0')
    {
        return true;
    }
    return false;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int get_random_value(int a, int b)
{
    return a + rand() % (abs(b - a) + 1);
}

void fill_array_with_random_numbers(int a, int b, int *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        array[i] = get_random_value(a, b);
    }
}

void print_array(int* array, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d : %d\n", i + 1, array[i]);
    }
}


//input checkers
enum input_check_status_codes
{
    invalid_input, 
    info_input_help,
    info_input,
    true_input
};

enum input_check_status_codes Input_checker(int argc, char* argv[], long long * a, long long * b)
{
    if (argc == 2) {
        if (!(strcmp(argv[1], "-help"))) {
            return info_input_help;
        }
        else if (!(strcmp(argv[1], "-info"))) {
            return info_input;
        }
        return invalid_input;
    }
    if (argc == 3)
    {
        if (is_number(argv[1], a) && is_number(argv[2], b))
        {
            return true_input;
        }
    }
    return invalid_input;
}
//-------------------------------------



//for first task
void swap_maxmin_elements(int *array, int len)
{
    int min_value = INT_MAX, max_value = INT_MIN;
    int max_pos, min_pos;

    for (int i = 0; i < len; i++)
    {
        if (array[i] > max_value)
        {
            max_value = array[i];
            max_pos = i;
        }
        if (array[i] < min_value)
        {
            min_value = array[i];
            min_pos = i;
        }
    }
    swap(&array[max_pos], &array[min_pos]);
}

int compare(const void *a, const void *b) 
{
    return (*(int*)a - *(int*)b); 
}


int bin_search(int *array, int length, int x)
{
    int left = 0;
    int right = length - 1;

    while (left < right - 1)
    {
        int mid = (left + right) * .5;
        if (x < array[mid])
        {
            right = mid;
        }
        else
        {
            left = mid;
        }
    }
    int diff_left_value = (array[left] - x);
    int diff_right_value = (array[left] - x);
    if (diff_left_value > diff_right_value)
    {
        return array[right];
    }
    return array[left];
}

void generate_c(int *a, int *b, int *c, int l1, int l2)
{
    for (int i = 0; i < l1; i++)
    {
        c[i] = a[i] + bin_search(b, l2, a[i]);
    }
}

#endif