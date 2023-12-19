#include "structs.c"

#define MIN_FOR_ARRAY 5

status_codes diff_file(const char *file1, const char *file2)
{
    int len1 = strlen(file1);
    int len2 = strlen(file2);
    int len = (len1 > len2) ? len1 : len2;

    char *temp1 = (char *)malloc(sizeof(char) * (len * PATH_MAX));
    if (temp1 == NULL)
        return STATUS_ERROR_MEMORY_ALLOCATION;
    char *temp2 = (char *)malloc(sizeof(char) * (len * PATH_MAX));
    if (temp2 == NULL)
    {
        free(temp1);
        return STATUS_ERROR_MEMORY_ALLOCATION;
    }

    if (realpath(file1, temp1) == NULL && realpath(file2, temp2) == NULL)
    {
        free(temp1);
        free(temp2);
        return STATUS_ERROR_FILE_SAME;
    }

    if (strcmp(temp1, temp2) == 0)
    {
        free(temp1);
        free(temp2);
        return STATUS_ERROR_FILE_SAME;
    }
    free(temp1);
    free(temp2);
    return STATUS_OK;
}

status_codes create_init_array(Array **arr, char name, int initial_capacity)
{
    *arr = (Array *)malloc(sizeof(Array));
    if (*arr == NULL)
        return STATUS_ERROR_MEMORY_ALLOCATION;

    (*arr)->name = toupper(name);
    (*arr)->length = 0;
    (*arr)->capacity = initial_capacity;
    (*arr)->data = (int *)malloc(initial_capacity * sizeof(int));
    if ((*arr)->data == NULL)
    {
        free(*arr);
        return STATUS_ERROR_MEMORY_ALLOCATION;
    }
    return STATUS_OK;
}

status_codes free_array(Array **arr)
{
    if (*arr == NULL)
        return STATUS_ERROR_EMPTY_ARRAY;
    free((*arr)->data);
    (*arr)->data = NULL;
    (*arr)->length = 0;
    (*arr)->capacity = 0;
    return STATUS_OK;
}

void clear_all_arrays(Array *array_storage[], size_t array_storage_size)
{
    for (size_t i = 0; i < array_storage_size; ++i)
    {
        if (array_storage[i] != NULL)
        {
            free_array(&array_storage[i]);
        }
    }
}

status_codes convert_str_to_long(const char *str, int *result)
{
    char *endptr;
    long int converted = strtol(str, &endptr, 10);

    if (endptr == str || (*endptr != '\0' && *endptr != '\n'))
    {
        return STATUS_ERROR_INVALID_FORMAT;
    }

    if (converted > INT_MAX || converted < INT_MIN)
    {
        return STATUS_ERROR_INT_OVERFLOW;
    }

    *result = (int)converted;
    return STATUS_OK;
}

status_codes load_array_from_file(Array *arr, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return STATUS_ERROR_FILE_OPEN;
    }

    char *buffer = NULL;
    size_t buffer_size = 0;

    while (getline(&buffer, &buffer_size, file) != -1)
    {
        char *token = strtok(buffer, " \t\n");

        while (token != NULL)
        {
            int converted_value;
            status_codes conversion_status = convert_str_to_long(token, &converted_value);

            if (conversion_status != STATUS_OK)
            {
                fclose(file);
                free(buffer);
                return conversion_status;
            }

            if (arr->length == arr->capacity)
            {
                int *tmp = (int *)realloc(arr->data, arr->capacity * sizeof(int) * 2);
                if (tmp == NULL)
                {
                    fclose(file);
                    free(buffer);
                    return STATUS_ERROR_MEMORY_ALLOCATION;
                }
                arr->capacity *= 2;
                arr->data = tmp;
            }

            arr->data[arr->length++] = converted_value;
            token = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    free(buffer);
    return STATUS_OK;
}

status_codes print_single_element(const Array *arr, int position)
{
    if (position < 0 || position >= arr->length)
        return STATUS_ERROR_INVALID_INDEX;
    printf("Element at position %d in Array %c: %d\n", position, arr->name, arr->data[position]);

    return STATUS_OK;
}

status_codes print_range(const Array *arr, int start, int end)
{
    if (start < 0 || start >= arr->length || end < start || end >= arr->length)
        return STATUS_ERROR_INVALID_INDEX;

    printf("Elements in the range [%d, %d] of Array %c: ", start, end, arr->name);
    for (int i = start; i <= end; i++)
    {
        printf("%d ", arr->data[i]);
    }
    printf("\n");

    return STATUS_OK;
}

status_codes print_all(const Array *arr)
{
    if (arr->length == 0)
        return STATUS_ERROR_EMPTY_ARRAY;

    printf("All elements of Array %c: ", arr->name);
    for (int i = 0; i < arr->length; i++)
    {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    return STATUS_OK;
}

status_codes save_array_to_file(const Array *arr, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
        return STATUS_ERROR_FILE_WRITE;

    for (int i = 0; i < arr->length; i++)
    {
        fprintf(file, "%d ", arr->data[i]);
    }

    fclose(file);
    return STATUS_OK;
}

status_codes fill_array_with_random(Array *arr, int count, int lb, int rb)
{
    if (lb > rb)
        return STATUS_ERROR_INVALID_RANGE;

    if (count > arr->capacity)
    {
        int *tmp = (int *)realloc(arr->data, count * sizeof(int));
        if (tmp == NULL)
            return STATUS_ERROR_MEMORY_ALLOCATION;
        arr->capacity = count;
        arr->data = tmp;
    }

    srand((unsigned int)time(NULL));

    for (int i = 0; i < count; i++)
    {
        arr->data[i] = rand() % (rb - lb + 1) + lb; // от lb до rb
    }

    arr->length = count;

    return STATUS_OK;
}

status_codes concatenate_arrays(Array *arrA, const Array *arrB)
{
    if (arrA->length + arrB->length > arrA->capacity)
    {
        int *tmp = (int *)realloc(arrA->data, (arrA->length + arrB->length) * sizeof(int));
        if (tmp == NULL)
            return STATUS_ERROR_MEMORY_ALLOCATION;
        arrA->capacity = arrA->length + arrB->length;
        arrA->data = tmp;
    }

    for (int i = 0; i < arrB->length; i++)
    {
        arrA->data[arrA->length + i] = arrB->data[i];
    }

    arrA->length += arrB->length;

    return STATUS_OK;
}

status_codes remove_elements(Array *arr, int start_index, int count)
{
    if (start_index < 0 || start_index >= arr->length || count < 0 || start_index + count > arr->length)
    {
        return STATUS_ERROR_INVALID_RANGE;
    }

    for (int i = start_index; i + count < arr->length; i++)
    {
        arr->data[i] = arr->data[i + count];
    }

    // Обновление длины массива
    arr->length -= count;

    return STATUS_OK;
}

status_codes copy_elements(const Array *a, Array *b, int lb, int rb)
{
    if (lb < 0 || lb >= a->length || rb < lb || rb >= a->length)
        return STATUS_ERROR_INVALID_RANGE;

    if (rb - lb + 1 > b->length)
    {
        int *tmp = (int *)realloc(b->data, (rb - lb + 1) * sizeof(int));
        if (tmp == NULL)
            return STATUS_ERROR_MEMORY_ALLOCATION;
        b->capacity = rb - lb + 1;
        b->length = rb - lb + 1;
        b->data = tmp;
    }

    for (int i = lb; i < rb + 1; i++)
    {
        b->data[i] = a->data[i];
    }

    return STATUS_OK;
}

int compare_ascending(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int compare_descending(const void *a, const void *b)
{
    return (*(int *)b - *(int *)a);
}

status_codes sort_array(Array *arr, char order)
{
    if (arr->length == 0)
        return STATUS_ERROR_EMPTY_ARRAY;

    int (*compare_function)(const void *, const void *);
    if (order == '+')
        compare_function = compare_ascending;
    else if (order == '-')
        compare_function = compare_descending;
    else
        return STATUS_ERROR_INVALID_COMMAND;

    qsort(arr->data, arr->length, sizeof(int), compare_function);

    return STATUS_OK;
}

void swap_elements(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int random_compare(const void *left, const void *right)
{
    int cmp = (rand() % INT_MAX) - (INT_MAX / 2);
    return cmp;
}

status_codes shuffle_array(Array *arr)
{
    if (arr->length == 0)
        return STATUS_ERROR_EMPTY_ARRAY;

    qsort(arr->data, arr->length, sizeof(int), random_compare);
    return STATUS_OK;
}

status_codes find_max(const Array *arr, int *max_value, int *max_index)
{
    if (arr->length == 0)
        return STATUS_ERROR_EMPTY_ARRAY;

    *max_value = arr->data[0];
    *max_index = 0;

    for (int i = 1; i < arr->length; ++i)
    {
        if (arr->data[i] > *max_value)
        {
            *max_value = arr->data[i];
            *max_index = i;
        }
    }
    return STATUS_OK;
}

status_codes find_min(const Array *arr, int *min_value, int *min_index)
{
    if (arr->length == 0)
        return STATUS_ERROR_EMPTY_ARRAY;

    *min_value = arr->data[0];
    *min_index = 0;

    for (int i = 1; i < arr->length; ++i)
    {
        if (arr->data[i] < *min_value)
        {
            *min_value = arr->data[i];
            *min_index = i;
        }
    }
    return STATUS_OK;
}

status_codes find_most_common(const Array *arr, int *most_common_value, int *most_common_count)
{
    if (arr->length == 0)
        return STATUS_ERROR_EMPTY_ARRAY;

    *most_common_value = arr->data[0];
    *most_common_count = 1;

    for (int i = 0; i < arr->length; ++i)
    {
        int current_value = arr->data[i];
        int current_count = 1;

        for (int j = i + 1; j < arr->length; ++j)
        {
            if (arr->data[j] == current_value)
                current_count++;
        }

        if (current_count > *most_common_count || (current_count == *most_common_count && current_value > *most_common_value))
        {
            *most_common_value = current_value;
            *most_common_count = current_count;
        }
    }
    return STATUS_OK;
}

status_codes find_average(const Array *arr, double *average)
{
    if (arr->length == 0)
    {
        return STATUS_ERROR_EMPTY_ARRAY;
    }

    int sum = 0;
    for (int i = 0; i < arr->length; ++i)
    {
        sum += arr->data[i];
    }

    *average = (double)sum / arr->length;

    return STATUS_OK;
}

status_codes find_max_deviation(const Array *arr, double *max_deviation)
{
    if (arr->length == 0)
        return STATUS_ERROR_EMPTY_ARRAY;

    double average;
    status_codes status_average = find_average(arr, &average);
    if (status_average != STATUS_OK)
        return status_average;

    *max_deviation = 0;

    for (int i = 0; i < arr->length; ++i)
    {
        double deviation = fabs((double)arr->data[i] - average);
        if (deviation > *max_deviation)
        {
            *max_deviation = deviation;
        }
    }
    return STATUS_OK;
}

void print_stats(const Array *arr)
{
    int max_value, max_index, min_value, min_index, most_common_value, most_common_count;
    double average, max_deviation;

    status_codes status_max = find_max(arr, &max_value, &max_index);
    status_codes status_min = find_min(arr, &min_value, &min_index);
    status_codes status_most_common = find_most_common(arr, &most_common_value, &most_common_count);
    status_codes status_average = find_average(arr, &average);
    status_codes status_deviation = find_max_deviation(arr, &max_deviation);

    printf("Array Stats %c :\n", arr->name);

    if (status_max == STATUS_OK)
        printf("Maximum value: %d at index %d\n", max_value, max_index);
    else
        print_status(status_max);

    if (status_min == STATUS_OK)
        printf("Minimum value: %d at index %d\n", min_value, min_index);
    else
        print_status(status_min);

    if (status_most_common == STATUS_OK)
        printf("Most common value: %d (occurs %d times)\n", most_common_value, most_common_count);
    else
        print_status(status_most_common);

    if (status_average == STATUS_OK)
        printf("Average: %.2f\n", average);
    else
        print_status(status_average);

    if (status_deviation == STATUS_OK)
        printf("Max deviation from average: %.2f\n", max_deviation);
    else
        print_status(status_deviation);
}

status_codes parsing(const char *filename_pars)
{
    FILE *file = fopen(filename_pars, "r");
    if (file == NULL)
        return STATUS_ERROR_FILE_OPEN;

    Array *array_storage[26] = {NULL};

    char *buffer = NULL;
    size_t buffer_size = 0;
    size_t buffer_index = 0;
    status_codes status;
    int c;

    while ((c = fgetc(file)) != EOF)
    {
        if (buffer == NULL || buffer_index >= buffer_size)
        {
            buffer_size += 128;
            char *tmp = realloc(buffer, buffer_size);
            if (tmp == NULL)
            {
                fclose(file);
                free(buffer);
                return STATUS_ERROR_MEMORY_ALLOCATION;
            }
            buffer = tmp;
        }

        buffer[buffer_index++] = c;

        if (c == ';')
        {
            buffer[buffer_index] = '\0';
            buffer_index = 0;

            char *command = strtok(buffer, " (\n\t");
            if (command == NULL)
            {
                print_status(STATUS_ERROR_INVALID_COMMAND);
                fclose(file);
                free(buffer);
                return STATUS_ERROR_INVALID_COMMAND;
            }

            if (strcmp(command, "Load") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t");
                if (array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name))
                {
                    fclose(file);
                    free(buffer);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char *filename = strtok(NULL, " ;\n\t");
                if (filename == NULL || diff_file(filename_pars, filename) != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    return STATUS_ERROR_FILE_OPEN;
                }

                if (strtok(NULL, " ,;\n\t()") != NULL)
                    return STATUS_ERROR_INVALID_COMMAND;

                Array *arr = NULL;
                int index = toupper(*array_name) - 'A';
                if (array_storage[index] != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_EXISTED;
                }
                if ((status = create_init_array(&arr, *array_name, MIN_FOR_ARRAY)) != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status;
                }
                array_storage[index] = arr;
                if ((status = load_array_from_file(arr, filename)) != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status;
                }
            }
            else if (strcmp(command, "Save") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t");
                if (array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char *filename = strtok(NULL, " ;\n\t");
                if (filename == NULL || diff_file(filename_pars, filename) != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                if (strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                Array *arr = NULL;
                int index = toupper(*array_name) - 'A';
                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }

                status_codes status = save_array_to_file(array_storage[index], filename);
                if (status != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status;
                }
            }
            else if (strcmp(command, "Rand") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t");
                if (array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int count, lb, rb;

                char *count_str = strtok(NULL, ", \n\t");
                status_codes status_count = convert_str_to_long(count_str, &count);
                if (status_count != STATUS_OK || count <= 0)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char *lb_str = strtok(NULL, ", \n\t");
                status_codes status_lb = convert_str_to_long(lb_str, &lb);
                if (status_lb != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char *rb_str = strtok(NULL, "; \n\t");
                status_codes status_rb = convert_str_to_long(rb_str, &rb);
                if (status_rb != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                if (strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index = toupper(*array_name) - 'A';
                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }

                status_codes status = fill_array_with_random(array_storage[index], count, lb, rb);
                if (status != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status;
                }
            }
            else if (strcmp(command, "Concat") == 0)
            {
                char *array_name_a = strtok(NULL, ", \n\t");
                if (array_name_a == NULL || strlen(array_name_a) != 1 || !isalpha(*array_name_a))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char *array_name_b = strtok(NULL, "; \n\t");
                if (array_name_b == NULL || strlen(array_name_b) != 1 || !isalpha(*array_name_b))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                if (strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index_a = toupper(*array_name_a) - 'A';
                int index_b = toupper(*array_name_b) - 'A';

                if (array_storage[index_a] == NULL || array_storage[index_b] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                status_codes status = concatenate_arrays(array_storage[index_a], array_storage[index_b]);
                if (status != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status;
                }
            }
            else if (strcmp(command, "Free") == 0)
            {
                char *array_name = strtok(NULL, ", )\n\t");
                if (array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                if (strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index = toupper(*array_name) - 'A';

                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }
                free_array(&array_storage[index]);
            }
            else if (strcmp(command, "Remove") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t");
                if (array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char *start_str = strtok(NULL, ", \n\t");
                char *end_str = strtok(NULL, ", \n\t;");

                if (start_str == NULL || end_str == NULL || strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index = toupper(*array_name) - 'A';

                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }

                int start, end;
                status_codes status_start = convert_str_to_long(start_str, &start);
                status_codes status_end = convert_str_to_long(end_str, &end);

                if (status_start != STATUS_OK || status_end != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return (status_start != STATUS_OK) ? status_start : status_end;
                }
                status_codes status_remove = remove_elements(array_storage[index], start, end);

                if (status_remove != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status_remove;
                }
            }
            else if (strcmp(command, "Copy") == 0)
            {
                char *source_name = strtok(NULL, ", \n\t");
                if (source_name == NULL || strlen(source_name) != 1 || !isalpha(*source_name))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char *start_str = strtok(NULL, ", \n\t");
                char *end_str = strtok(NULL, ", \n\t");
                char *dest_name = strtok(NULL, " ,;\n\t");

                if (start_str == NULL || end_str == NULL || dest_name == NULL || strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int source_index = toupper(*source_name) - 'A';
                int dest_index = toupper(*dest_name) - 'A';

                if (array_storage[source_index] == NULL || array_storage[dest_index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int start, end;
                status_codes status_start = convert_str_to_long(start_str, &start);
                status_codes status_end = convert_str_to_long(end_str, &end);

                if (status_start != STATUS_OK || status_end != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return (status_start != STATUS_OK) ? status_start : status_end;
                }
                status_codes status_copy = copy_elements(array_storage[source_index], array_storage[dest_index], start, end);

                if (status_copy != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status_copy;
                }
            }
            else if (strcmp(command, "Sort") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t");
                char *order_str = strtok(NULL, " ;\n\t");

                if (array_name == NULL || order_str == NULL || strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index = toupper(*array_name) - 'A';

                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }

                if (strlen(order_str) != 1 || order_str[0] != '+' || order_str[0] != '-')
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_FORMAT;
                }

                status_codes status_sort = sort_array(array_storage[index], order_str[0]);

                if (status_sort != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return status_sort;
                }
            }
            else if (strcmp(command, "Shuffle") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t");

                if (array_name == NULL || strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index = toupper(*array_name) - 'A';

                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }

                if (shuffle_array(array_storage[index]) != STATUS_OK)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_EMPTY_ARRAY;
                }
            }
            else if (strcmp(command, "Stats") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t;");

                if (array_name == NULL || strtok(NULL, " ,;\n\t()") != NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index = toupper(*array_name) - 'A';

                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }

                print_stats(array_storage[index]);
            }
            else if (strcmp(command, "Print") == 0)
            {
                char *array_name = strtok(NULL, ", \n\t");
                if (array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name))
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int index = toupper(*array_name) - 'A';

                if (array_storage[index] == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_ARRAY_NOT_EXISTED;
                }

                char *next = strtok(NULL, ", \n\t;");
                if (next == NULL)
                {
                    fclose(file);
                    free(buffer);
                    clear_all_arrays(array_storage, 26);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                int start = 0, end = 0;
                char *next2 = strtok(NULL, ", \n\t;");

                if (strcmp(next, "all") == 0)
                {
                    if (print_all(array_storage[index]) != STATUS_OK)
                        return STATUS_ERROR_EMPTY_ARRAY;
                }
                else if (next2 == NULL)
                {
                    int crnt_indx;
                    status_codes status_ = convert_str_to_long(next, &crnt_indx);
                    if (status_ != STATUS_OK)
                    {
                        fclose(file);
                        free(buffer);
                        clear_all_arrays(array_storage, 26);
                        return status_;
                    }
                    status = print_single_element(array_storage[index], crnt_indx);
                    if (status != STATUS_OK)
                        return status;
                }
                else
                {
                    status_codes status_start = convert_str_to_long(next, &start);
                    status_codes status_end = convert_str_to_long(next2, &end);

                    if (status_start != STATUS_OK || status_end != STATUS_OK)
                    {
                        fclose(file);
                        free(buffer);
                        clear_all_arrays(array_storage, 26);
                        return (status_start != STATUS_OK) ? status_start : status_end;
                    }

                    status_end = print_range(array_storage[index], start, end);
                    if (status_end != STATUS_OK)
                        return status_end;
                }
            }

            free(buffer);
            buffer = NULL;
            buffer_size = 0;
        }
    }

    fclose(file);
    free(buffer);
    clear_all_arrays(array_storage, 26);
    return STATUS_OK;
}

int main()
{
    status_codes status = parsing("file.txt");

    if (status != STATUS_OK)
    {
        print_status(status);
    }

    return status;
}