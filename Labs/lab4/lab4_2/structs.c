#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define STR_SIZE 256

typedef enum 
{
    STATUS_OK,
    STATUS_ERROR_FILE_OPEN,
    STATUS_ERROR_FILE_WRITE,
    STATUS_ERROR_FILE_READ,
    STATUS_ERROR_MEMORY_ALLOCATION,
    STATUS_ERROR_INVALID_COMMAND,
    STATUS_ERROR_INVALID_INDEX,
    STATUS_ERROR_INVALID_RANGE,
    STATUS_ERROR_EMPTY_ARRAY,
    STATUS_ERROR_INT_OVERFLOW,
    STATUS_ERROR_INVALID_FORMAT
} status_codes;

void print_status(status_codes status) 
{
    switch (status) 
    {
        case STATUS_OK:
            printf("Operation completed successfully.\n");
            break;
        case STATUS_ERROR_FILE_OPEN:
            fprintf(stderr, "Error: Unable to open the file.\n");
            break;
        case STATUS_ERROR_FILE_WRITE:
            fprintf(stderr, "Error: Unable to write to the file.\n");
            break;
        case STATUS_ERROR_FILE_READ:
            fprintf(stderr, "Error: Unable to read the file.\n");
            break;
        case STATUS_ERROR_MEMORY_ALLOCATION:
            fprintf(stderr, "Error: Unable to allocate memory.\n");
            break;
        case STATUS_ERROR_INVALID_COMMAND:
            fprintf(stderr, "Error: Invalid command.\n");
            break;
        case STATUS_ERROR_INVALID_INDEX:
            fprintf(stderr, "Error: Invalid index.\n");
            break;
        case STATUS_ERROR_INVALID_RANGE:
            fprintf(stderr, "Error: Invalid range.\n");
            break;
        case STATUS_ERROR_EMPTY_ARRAY:
            fprintf(stderr, "Error: The array is empty.\n");
            break;
        case STATUS_ERROR_INT_OVERFLOW:
            fprintf(stderr, "Error: Integer overflow.\n");
            break;
        case STATUS_ERROR_INVALID_FORMAT:
            fprintf(stderr, "Error: Invalid format.\n");
            break;
        default:
            fprintf(stderr, "Unknown status.\n");
            break;
    }
}

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_error_oppening,
    cmd_success
} status_cmd;

typedef struct Array {
    char name;
    int* data;
    int length;
    int capacity;
} Array;
