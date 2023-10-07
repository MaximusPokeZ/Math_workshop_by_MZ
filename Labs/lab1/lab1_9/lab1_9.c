#include "functions.h"


int main(int argc, char *argv[])
{
    srand(time(NULL));
    int random[FIXED_SIZE_ARRAY];
    long long a, b;



    switch (Input_checker(argc, argv, &a, &b))
    {
        case invalid_input:
            printf("Error : invalid input!\n");
            break;
        case info_input:
            print_info();
            break;
        case info_input_help:
            print_help(argv[0]);
            break;
        case true_input:
            fill_array_with_random_numbers(a, b, random, FIXED_SIZE_ARRAY);
            printf("\nRandom-generated array:\n");
            print_array(random, FIXED_SIZE_ARRAY);
            swap_maxmin_elements(random, FIXED_SIZE_ARRAY);
            printf("\nOur array with changes (max <-> min): \n\n");
            print_array(random, FIXED_SIZE_ARRAY);
            break;
    }
    printf("\n\nTask 2:\n");

    int len_array1 = get_random_value(MIN_RANGE, MAX_RANGE);
    int len_array2 = get_random_value(MIN_RANGE, MAX_RANGE);
    int *array_a = (int *)malloc(sizeof(int) * len_array1);
    int *array_b = (int *)malloc(sizeof(int) * len_array2);
    int *array_c = (int *)malloc(sizeof(int) * len_array1);

    if (array_a == NULL || array_b == NULL || array_c == NULL)
    {
        printf("Error: memory allocation problem\n");
    }
    else
    {
        fill_array_with_random_numbers(-1000, 1000, array_a, len_array1);
        fill_array_with_random_numbers(-1000, 1000, array_b, len_array2);
        printf("\nRandom-generated array A: \n");
        print_array(array_a, len_array1);
        printf("\nRandom-generated array B: \n");
        print_array(array_b, len_array2);
        qsort(array_b, len_array2, sizeof(int), compare);
        generate_c(array_a, array_b, array_c, len_array1, len_array2);
        printf("\nArray generated from A and B:\n");
        print_array(array_c, len_array1);
    }
    free(array_a);
    free(array_b);
    free(array_c);
    return 0;
}