#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#define BUFF_SIZE 50

typedef enum 
{
    SUCCESS,
    INVALID_INPUT,
    OPENING_ERROR,
    DIVISION_BY_ZERO,
    INVALID_ARGUMENT,
    MEMORY_ALLOCATION_ERROR,
    UNINITIALIZED_VARIABLE,
    CONVERSION_ERROR,
    VARIABLE_NOT_FOUND
} status_codes;

void print_status_codes(status_codes status)
{
    switch (status)
    {
        case SUCCESS:
            printf("Operation completed successfully.\n");
            break;
        case INVALID_INPUT:
            printf("Invalid input format.\n");
            break;
        case OPENING_ERROR:
            printf("Error opening the file.\n");
            break;
        case DIVISION_BY_ZERO:
            printf("Division by zero error.\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error.\n");
            break;
        case INVALID_ARGUMENT:
            printf("Invalid argument.\n");
            break;
        case UNINITIALIZED_VARIABLE:
            printf("ERROR! Using uninitialized variable.\n");
            break;
        case CONVERSION_ERROR:
            printf("Error converting string to number.\n");
            break;
        case VARIABLE_NOT_FOUND:
            printf("Variable not found in the memory array or error with digit\n");
            break;
        default:
            printf("An unexpected error occurred.\n");
            break;
    }
}

typedef enum 
{
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    MODULUS,
    ASSIGNMENT,
    PRINT,
    NONE
} Operation_status;


Operation_status parse_operation(const char op)
{
    switch (op)
    {
        case '+':
            return ADDITION;
        case '-':
            return SUBTRACTION;
        case '*':
            return MULTIPLICATION;
        case '/':
            return DIVISION;
        case '%':
            return MODULUS;
        case '=':
            return ASSIGNMENT;
        default:
            return NONE;
    }
}


int is_operator(int ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%');
}

typedef struct 
{
    char* name;
    long long value;
} Memory_cell;

typedef struct 
{
    unsigned long long size;
    unsigned long long cnt;
    Memory_cell* variables;
} Cell_array;

int compare_memory_cells(const void* a, const void* b)
{
    return strcmp(((Memory_cell*)a)->name, ((Memory_cell*)b)->name);
}


status_codes cells_arr_init(Cell_array* ptr)
{
    if (ptr == NULL) return INVALID_ARGUMENT;

    ptr->size = 5;
    ptr->cnt = 0;
    ptr->variables = NULL;
    ptr->variables = (Memory_cell*)malloc(ptr->size * sizeof(Memory_cell));
    if (ptr->variables == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }
    return SUCCESS;
}

void free_memory(Cell_array* memory)
{
    if (memory == NULL || memory->variables == NULL) return;

    for (unsigned long long i = 0; i < memory->cnt; i++)
    {
        if (memory->variables[i].name != NULL)
        {
            free(memory->variables[i].name);
            memory->variables[i].name = NULL;
        }
    }

    free(memory->variables);
}


Memory_cell* find_memory_cell(Cell_array* memory, const char* name)
{
    if (memory->variables == NULL || memory->cnt == 0) return NULL;

    long long low = 0;
    long long high = memory->cnt - 1;

    while (low <= high)
    {
        long long mid = (low + high) / 2;
        int cmp = strcmp(memory->variables[mid].name, name);

        if (cmp == 0)
        {
            return &memory->variables[mid];
        }
        else if (cmp < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return NULL;
}

status_codes insert(Cell_array* memory, const char* name, const long long value)
{
    if (memory == NULL || name == NULL)
    {
        return INVALID_ARGUMENT;
    }

    Memory_cell* existing_variable = find_memory_cell(memory, name);
    if (existing_variable != NULL)
    {
        existing_variable->value = value;
        return SUCCESS;
    }

    if (memory->cnt == memory->size)
    {
        memory->size = (memory->size == 0) ? 5 : memory->size * 2;
        Memory_cell* ptr = (Memory_cell*)realloc(memory->variables, memory->size * sizeof(Memory_cell));
        if (ptr == NULL)
        {
            return MEMORY_ALLOCATION_ERROR;
        }
        memory->variables = ptr;
    }

    memory->variables[memory->cnt].name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    if (memory->variables[memory->cnt].name == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy(memory->variables[memory->cnt].name, name);
    memory->variables[memory->cnt].value = value;
    memory->cnt++;
    qsort(memory->variables, memory->cnt, sizeof(Memory_cell), compare_memory_cells);
    return SUCCESS;
}


status_codes perform_operation(long long* result, Operation_status operation, long long value1, long long value2)
{
    switch (operation)
    {
        case ADDITION:
            *result = value1 + value2;
            return SUCCESS;
        case SUBTRACTION:
            *result = value1 - value2;
            return SUCCESS;
        case MULTIPLICATION:
            *result = value1 * value2;
            return SUCCESS;
        case DIVISION:
            if (value2 != 0)
            {
                *result = value1 / value2;
                return SUCCESS;
            }
            else
            {
                return DIVISION_BY_ZERO;
            }
        case MODULUS:
            if (value2 != 0)
            {
                *result = value1 % value2;
                return SUCCESS;
            }
            else
            {
                return DIVISION_BY_ZERO;
            }
        default:
            return INVALID_ARGUMENT;
    }
}

status_codes parse_long(const char* str, long long* result)
{
    char* endptr;
    *result = strtoll(str, &endptr, 10);

    if (*endptr != '\0')
    {
        return CONVERSION_ERROR;
    }

    return SUCCESS;
}

status_codes parse_instruction_parts(FILE* input, char** name, char** op, char** val_str1, char** val_str2) 
{
    size_t name_size = BUFF_SIZE;
    *name = (char*)malloc(name_size * sizeof(char));
    if (*name == NULL) return MEMORY_ALLOCATION_ERROR;

    size_t op_size = BUFF_SIZE;
    *op = (char*)malloc(op_size * sizeof(char));
    if (*op == NULL) return MEMORY_ALLOCATION_ERROR;

    size_t val_size1 = BUFF_SIZE;
    *val_str1 = (char*)malloc(val_size1 * sizeof(char));
    if (*val_str1 == NULL) return MEMORY_ALLOCATION_ERROR;

    size_t val_size2 = BUFF_SIZE;
    *val_str2 = (char*)malloc(val_size2 * sizeof(char));
    if (*val_str2 == NULL) return MEMORY_ALLOCATION_ERROR;

    size_t name_index = 0, op_index = 0, val_index1 = 0, val_index2 = 0;
    int ch;

    while ((ch = fgetc(input)) != EOF && (isspace(ch) || ch == ';'));

    while (isalpha(ch)) 
    {
        if (name_index >= name_size - 1) 
        {
            char* temp = (char*)realloc(*name, (name_size + BUFF_SIZE) * sizeof(char));
            if (temp == NULL) return MEMORY_ALLOCATION_ERROR;

            *name = temp;
            name_size += BUFF_SIZE;
        }
        (*name)[name_index++] = (char)ch;
        ch = fgetc(input);
    }
    (*name)[name_index] = '\0';

    while (isspace(ch)) { ch = fgetc(input); }

    if (ch == '=')
    {
        (*op)[0] = '='; (*op)[1] = '\0';
        ch = fgetc(input);
        while (isspace(ch)) { ch = fgetc(input); }
    } 
    else 
    {
        if (strcmp(*name, "print") == 0) 
        {
            while (isspace(ch)) { ch = fgetc(input); }
            if (ch == ';') 
            {
                (*val_str1)[0] = '\0'; (*val_str2)[0] = '\0';
                return SUCCESS;
            } 
            else 
            {
                while (!isspace(ch) && ch != ';') 
                {
                    if (val_index1 >= val_size1 - 1) 
                    {
                        char* temp = (char*)realloc(*val_str1, (val_size1 + BUFF_SIZE) * sizeof(char));
                        if (temp == NULL) return MEMORY_ALLOCATION_ERROR;
                        *val_str1 = temp;
                        val_size1 += BUFF_SIZE;
                    }

                    (*val_str1)[val_index1++] = (char)ch;
                    ch = fgetc(input);
                    if (is_operator(ch)) return INVALID_INPUT;
                }
                (*val_str1)[val_index1] = '\0';

                while (isspace(ch)) { ch = fgetc(input); }

                if (ch != EOF && ch != ';') return INVALID_INPUT;
                return SUCCESS;
            }
        } 
        else return INVALID_INPUT;
    }

    int flag = 2;
    while ((isdigit(ch) || isalpha(ch) || ch == '-') && flag) 
    {
        if (ch == '-') flag--;
        if (val_index1 >= val_size1 - 1) 
        {
            char* temp = (char*)realloc(*val_str1, (val_size1 + BUFF_SIZE) * sizeof(char));
            if (temp == NULL) return MEMORY_ALLOCATION_ERROR;
            *val_str1 = temp;
            val_size1 += BUFF_SIZE;
        }
        (*val_str1)[val_index1++] = (char)ch;
        ch = fgetc(input);
    }
    (*val_str1)[val_index1] = '\0';

    while (isspace(ch)) { ch = fgetc(input); }

    if (ch == ';') return SUCCESS;
    else if (!is_operator(ch)) return INVALID_INPUT;

    (*op)[0] = (char)ch; (*op)[1] = '\0';
    ch = fgetc(input);

    while (isspace(ch)) { ch = fgetc(input); }

    while ((isdigit(ch) || isalpha(ch))) 
    {
        if (val_index2 >= val_size2 - 1) 
        {
            char* temp = (char*)realloc(*val_str2, (val_size2 + BUFF_SIZE) * sizeof(char));
            if (temp == NULL) return MEMORY_ALLOCATION_ERROR;
            *val_str2 = temp;
            val_size2 += BUFF_SIZE;
        }
        (*val_str2)[val_index2++] = (char)ch;
        ch = fgetc(input);
    }
    (*val_str2)[val_index2] = '\0';

    while (isspace(ch)) { ch = fgetc(input); }

    if (ch == ';') return SUCCESS;
    else return INVALID_INPUT;
}

status_codes parse_instruction(FILE* input, Cell_array* memory) 
{
    int ch;
    while ((ch = fgetc(input)) != EOF) 
    {
        if (isspace(ch) || ch == ';') { continue; }
        ungetc(ch, input);

        char* name = NULL;
        char* op = NULL;
        char* val_str1 = NULL;
        char* val_str2 = NULL;

        status_codes status = SUCCESS;
        status = parse_instruction_parts(input, &name, &op, &val_str1, &val_str2);
        if (status != SUCCESS) 
        {
            free(name); free(op); free(val_str1); free(val_str2);
            return status;
        }

        if (strcmp(name, "print") == 0 && op[0] != '=') 
        {
            if (strlen(val_str1) == 0 && strlen(val_str2) == 0) 
            {
                if (memory->cnt == 0) printf("Is empty\n");
                else
                {
                    for (unsigned long long i = 0; i < memory->cnt; i++) 
                    {
                        printf("%s: %lld\n", memory->variables[i].name, memory->variables[i].value);
                    }
                }
            } 
            else 
            {
                long long value;
                if (strlen(val_str2) == 0) 
                {
                    Memory_cell* variable = find_memory_cell(memory, val_str1);
                    if (variable != NULL) 
                    {
                        printf("%s: %lld\n", val_str1, variable->value);
                    } 
                    else 
                    {
                        status = parse_long(val_str1, &value);
                        if (status != SUCCESS) 
                        {
                            free(name); free(op); free(val_str1); free(val_str2);
                            return VARIABLE_NOT_FOUND;
                        }
                        printf("%lld\n", value);
                    }
                }
            } 
        }
        else if (op[0] == '=')
        {
            long long value;
            Memory_cell* variable_to_add = find_memory_cell(memory, val_str1);
            if (variable_to_add != NULL) value = variable_to_add->value;
            else
            {
                status = parse_long(val_str1, &value);
                if (status != SUCCESS)
                {
                    free(name); free(op); free(val_str1); free(val_str2);
                    return VARIABLE_NOT_FOUND;
                }
            }
            status = insert(memory, name, value);
            if (status != SUCCESS)
            {
                free(name); free(op); free(val_str1); free(val_str2);
                return status;
            }
        }
        else if (is_operator(op[0]))
        {
            long long value1, value2;
            Memory_cell* variable1 = find_memory_cell(memory, val_str1);
            Memory_cell* variable2 = find_memory_cell(memory, val_str2);
            status_codes parse_status1, parse_status2;

            if (variable1 != NULL)
            {
                value1 = variable1->value;
            }
            else
            {
                parse_status1 = parse_long(val_str1, &value1);
                if (parse_status1 != SUCCESS)
                {
                    free(name); free(op); free(val_str1); free(val_str2);
                    return VARIABLE_NOT_FOUND;;
                }
            }

            if (variable2 != NULL)
            {
                value2 = variable2->value;
            }
            else
            {
                parse_status2 = parse_long(val_str2, &value2);
                if (parse_status2 != SUCCESS)
                {
                    free(name); free(op); free(val_str1); free(val_str2);
                    return VARIABLE_NOT_FOUND;
                }
            }
            Operation_status operation = parse_operation(op[0]);
            if (operation != NONE)
            {
                long long result;
                status = perform_operation(&result, operation, value1, value2);
                if (status != SUCCESS)
                {
                    free(name); free(op); free(val_str1); free(val_str2);
                    return status;
                }
                status = insert(memory, name, result);
                if (status != SUCCESS)
                {
                    free(name); free(op); free(val_str1); free(val_str2);
                    return status;
                }
            }
            else
            {
                free(name); free(op); free(val_str1); free(val_str2);
                return INVALID_INPUT;
            }
        }
        free(name); free(op); free(val_str1); free(val_str2);
    }
    return SUCCESS;
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return INVALID_ARGUMENT;
    }

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) 
    {
        print_status_codes(OPENING_ERROR);
        return OPENING_ERROR;
    }

    status_codes status;
    Cell_array memory;
    if ((status = cells_arr_init(&memory)) != SUCCESS) 
    {
        print_status_codes(status);
        fclose(input);
        return status;
    }

    status = parse_instruction(input, &memory);
    print_status_codes(status);
    free_memory(&memory);
    fclose(input);
    return 0;
}

