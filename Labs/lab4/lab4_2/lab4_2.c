#include "structs.c"


status_codes create_init_array(Array** arr, char name, int initial_capacity) 
{
    *arr = (Array*)malloc(sizeof(Array));
    if (*arr == NULL) return STATUS_ERROR_MEMORY_ALLOCATION;

    (*arr)->name = toupper(name);
    (*arr)->length = 0;
    (*arr)->capacity = initial_capacity;
    (*arr)->data = (int*)malloc(initial_capacity * sizeof(int));
    if ((*arr)->data == NULL) 
    {
        free(*arr);
        return STATUS_ERROR_MEMORY_ALLOCATION;
    }
    return STATUS_OK;
}

status_codes free_array(Array** arr) 
{
    if (*arr == NULL) return STATUS_ERROR_EMPTY_ARRAY;
    free((*arr)->data);
    (*arr)->data = NULL;
    (*arr)->length = 0;
    (*arr)->capacity = 0;
    return STATUS_OK;
}

status_codes convert_str_to_long(const char* str, int* result) 
{
    char* endptr;
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

status_codes load_array_from_file(Array* arr, const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        return STATUS_ERROR_FILE_OPEN;
    }

    char* buffer = NULL;
    size_t buffer_size = 0;

    while (getline(&buffer, &buffer_size, file) != -1) 
    {
        char* token = strtok(buffer, " \t\n");

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
                int* tmp = (int*)realloc(arr->data, arr->capacity * sizeof(int) * 2);
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


status_codes print_single_element(const Array* arr, int position) 
{
    if (position < 0 || position >= arr->length) return STATUS_ERROR_INVALID_INDEX;
    printf("Element at position %d in Array %c: %d\n", position, arr->name, arr->data[position]);

    return STATUS_OK;
}

status_codes print_range(const Array* arr, int start, int end) 
{
    if (start < 0 || start >= arr->length || end < start || end >= arr->length) return STATUS_ERROR_INVALID_INDEX;

    printf("Elements in the range [%d, %d] of Array %c: ", start, end, arr->name);
    for (int i = start; i <= end; i++) { printf("%d ", arr->data[i]); }
    printf("\n");

    return STATUS_OK;
}

status_codes print_all(const Array* arr) 
{
    if (arr->length == 0) return STATUS_ERROR_EMPTY_ARRAY;

    printf("All elements of Array %c: ", arr->name);
    for (int i = 0; i < arr->length; i++) { printf("%d ", arr->data[i]); }
    printf("\n");
    return STATUS_OK;
}

status_codes save_array_to_file(const Array* arr, const char* filename) 
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) return STATUS_ERROR_FILE_WRITE;

    for (int i = 0; i < arr->length; i++) { fprintf(file, "%d ", arr->data[i]); }

    fclose(file);
    return STATUS_OK;
}

status_codes fill_array_with_random(Array* arr, int count, int lb, int rb) 
{
    if (lb > rb) return STATUS_ERROR_INVALID_RANGE;

    if (count > arr->capacity) 
    {
        int* tmp = (int*)realloc(arr->data, count * sizeof(int));
        if (tmp == NULL) return STATUS_ERROR_MEMORY_ALLOCATION;
        arr->capacity = count;
        arr->data = tmp;
    }

    srand((unsigned int)time(NULL)); 

    for (int i = 0; i < count; i++) 
    {
        arr->data[i] = rand() % (rb - lb + 1) + lb; //от lb до rb
    }

    arr->length = count;

    return STATUS_OK;
}

status_codes concatenate_arrays(Array* arrA, const Array* arrB) 
{
    if (arrA->length + arrB->length > arrA->capacity) 
    {
        int* tmp = (int*)realloc(arrA->data, (arrA->length + arrB->length) * sizeof(int));
        if (tmp == NULL) return STATUS_ERROR_MEMORY_ALLOCATION;
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

status_codes remove_elements(Array* arr, int start_index, int count) 
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

status_codes copy_elements (const Array* a, Array * b, int lb, int rb)
{
    if (lb < 0 || lb >= a->length || rb < lb || rb >= a->length) return STATUS_ERROR_INVALID_RANGE;

    if (rb - lb + 1 > b->length)
    {
        int* tmp = (int*)realloc(b->data, (rb - lb + 1) * sizeof(int));
        if (tmp == NULL) return STATUS_ERROR_MEMORY_ALLOCATION;
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

int compare_ascending(const void* a, const void* b) 
{
    return (*(int*)a - *(int*)b);
}

int compare_descending(const void* a, const void* b) 
{
    return (*(int*)b - *(int*)a);
}

status_codes sort_array(Array* arr, char order) 
{
    if (arr->length == 0) return STATUS_ERROR_EMPTY_ARRAY;

    int (*compare_function)(const void*, const void*);
    if (order == '+') compare_function = compare_ascending;
    else if (order == '-') compare_function = compare_descending;
    else return STATUS_ERROR_INVALID_COMMAND;

    qsort(arr->data, arr->length, sizeof(int), compare_function);

    return STATUS_OK;
}

void swap_elements(int* a, int* b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

status_codes shuffle_array(Array* arr) 
{
    if (arr->length == 0) return STATUS_ERROR_EMPTY_ARRAY;

    srand((unsigned int)time(NULL));

    for (int i = arr->length - 1; i > 0; i--) 
    {
        int j = rand() % (i + 1); // ot 0 do i
        swap_elements(&arr->data[i], &arr->data[j]);
    }

    return STATUS_OK;
}

status_codes find_max(const Array* arr, int* max_value, int* max_index) 
{
    if (arr->length == 0) return STATUS_ERROR_EMPTY_ARRAY;

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

status_codes find_min(const Array* arr, int* min_value, int* min_index) 
{
    if (arr->length == 0) return STATUS_ERROR_EMPTY_ARRAY;

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

status_codes find_most_common(const Array* arr, int* most_common_value, int* most_common_count) 
{
    if (arr->length == 0) return STATUS_ERROR_EMPTY_ARRAY;

    *most_common_value = arr->data[0];
    *most_common_count = 1;

    for (int i = 0; i < arr->length; ++i) 
    {
        int current_value = arr->data[i];
        int current_count = 1;

        for (int j = i + 1; j < arr->length; ++j) 
        {
            if (arr->data[j] == current_value) current_count++;
        }

        if (current_count > *most_common_count || (current_count == *most_common_count && current_value > *most_common_value)) 
        {
            *most_common_value = current_value;
            *most_common_count = current_count;
        }
    }
    return STATUS_OK;
}

status_codes find_average(const Array* arr, double* average)
 {
    if (arr->length == 0) {
        return STATUS_ERROR_EMPTY_ARRAY;
    }

    int sum = 0;
    for (int i = 0; i < arr->length; ++i) {
        sum += arr->data[i];
    }

    *average = (double)sum / arr->length;

    return STATUS_OK;
}

status_codes find_max_deviation(const Array* arr, double* max_deviation) 
{
    if (arr->length == 0) return STATUS_ERROR_EMPTY_ARRAY;

    double average;
    status_codes status_average = find_average(arr, &average);
    if (status_average != STATUS_OK) return status_average;

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

status_codes parsing(const char* filename_pars) 
{
    FILE* file = fopen(filename_pars, "r");
    if (file == NULL) return STATUS_ERROR_FILE_OPEN;

    char* buffer = NULL;
    size_t buffer_size = 0; 
    size_t buffer_index = 0;
    status_codes status;
    int c; 

    while ((c = fgetc(file)) != EOF) 
    {
        if (buffer == NULL || buffer_index >= buffer_size) 
        {
            buffer_size += 128;
            char* tmp = realloc(buffer, buffer_size);
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

            char* command = strtok(buffer, " \n\t");
            if (command == NULL) 
            {
                print_status(STATUS_ERROR_INVALID_COMMAND);
                fclose(file);
                free(buffer);
                return STATUS_ERROR_INVALID_COMMAND;
            }

            if (strcmp(command, "Load") == 0) 
            {
                char* array_name = strtok(NULL, ", \n\t");
                if (array_name == NULL) {
                    fclose(file);
                    free(buffer);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                char* filename = strtok(NULL, " ;\n\t");
                if (filename == NULL) {
                    fclose(file);
                    free(buffer);
                    return STATUS_ERROR_INVALID_COMMAND;
                }

                if (strtok(NULL, " ,;\n\t()") != NULL) return STATUS_ERROR_INVALID_COMMAND;

                

            }
            else if (strcmp(command, "Save") == 0) {
                // Обработка команды Save
                // Реализуйте соответствующую логику
            }
            // Добавьте обработку других команд

            // Очистка буфера для следующей команды
            free(buffer);
            buffer = NULL;
            buffer_size = 0;
        }
    }

    fclose(file);
    free(buffer);

    return STATUS_OK;
}

int main()
{
    Array * a = NULL;
    status_codes status;
    status = create_init_array(&a, 'A', 100);
    if (status != STATUS_OK)
    {
        return status;
    }

    status = load_array_from_file(a, "in.txt");
    if (status != STATUS_OK)
    {
        free_array(&a);
        print_status(status);
        return status;
    }

    status = print_single_element(a, 3);
    print_status(status);

    status = print_range(a, 4, 6);
    print_status(status);

    status = print_all(a);
    print_status(status);

    Array * b = NULL;

    create_init_array(&b, 'B', 10);
    fill_array_with_random(b, 15, -5, 5);

    status = print_all(b);
    print_status(status);

    save_array_to_file(b, "kkk.txt");

    concatenate_arrays(a, b);
    print_all(a);

    free_array(&a);
    free_array(&b);

    parsing("file.txt");

    return status;
}