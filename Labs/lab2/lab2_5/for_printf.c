#include "functions.c"

#define STRING_SIZE 256

void print_errors (enum transfer_to_status_codes status, FILE* stream, int * result, char ** string)
{
    if (status == out_of_range)
    {
        if (stream == NULL) *result += sprintf(*string, "Number out of range for Roman numerals\n");
        else *result += fprintf(stream, "Number out of range for Roman numerals\n");
    }
    else
    {
        if (stream == NULL) *result += sprintf(*string,"Memory allocation problem\n");
        else *result += fprintf(stream, "Memory allocation problem\n");
    }
}


int check_flags(FILE* stream, const char* format, va_list* ptr, char ** string)
{
    int result = 0;
    char* buffer_ptr;
    if (stream == NULL) buffer_ptr = *string;
    while (*format)
    {
        if (*format == '%' && *(format + 1) == 'R' && *(format + 2) == 'o')
        {
            format += 3; 
            int num = va_arg(*ptr, int);
            char* roman = NULL;
            enum transfer_to_status_codes roman_status = int_transfer_to_roman(num, &roman);
            if (roman_status == ok_transfer)
            {
                if (stream == NULL) {result += sprintf(buffer_ptr, "%s", roman); buffer_ptr += strlen(roman);}
                else result += fprintf(stream, "%s", roman);
                free(roman);
            }
            else
            {
                print_errors(roman_status, stream, &result, &buffer_ptr);
                break;
            }
        }
        else if (*format == '%' && *(format + 1) == 'Z' && *(format + 2) == 'r')
        {
            format += 3; 
            unsigned int num = va_arg(*ptr, int);
            char* cyckendorf = NULL;
            enum transfer_to_status_codes cyckendorf_status = transfer_cyckendorf(num, &cyckendorf);
            if (cyckendorf_status == ok_transfer)
            {
                if (stream == NULL) {result += sprintf(buffer_ptr, "%s", cyckendorf);  buffer_ptr += strlen(cyckendorf);}
                else result += fprintf(stream, "%s", cyckendorf);
                free(cyckendorf);
            }
            else
            {
                print_errors(cyckendorf_status, stream, &result, &buffer_ptr);
                break;
            }
        }
        else if (*format == '%' && *(format + 1) == 'C' && (*(format + 2) == 'v' || *(format + 2) == 'V'))
        {   
            char flag = *(format + 2);
            format += 3; 
            int num = va_arg(*ptr, int), base = va_arg(*ptr, int), counter;
            char* transfer = NULL;
            bool sign;
            enum transfer_to_status_codes transfer_status = int_transfer_to_base(num, &transfer, base, flag, &counter, &sign);
            if (transfer_status == ok_transfer)
            {
                for (int i = SIZE_TO_BASE - counter - 1; i < SIZE_TO_BASE - 1 + sign; i++) 
                {
                    if (stream == NULL) {result += sprintf(buffer_ptr, "%c", transfer[i]); buffer_ptr++;}
                    else result += fprintf(stream, "%c", transfer[i]);
                }
                free(transfer);
            }
            else
            {
                print_errors(transfer_status, stream, &result, &buffer_ptr);
                break;
            }
        }
        else if (*format == '%' && ((*(format + 1) == 't' && *(format + 2) == 'o') || (*(format + 1) == 'T' && *(format + 2) == 'O')))
        {   
            char flag = *(format + 1);
            format += 3; 
            char* num = va_arg(*ptr, char*);
            int base = va_arg(*ptr, int);
            long long transfer;
            enum transfer_to_status_codes transfer_status = ss_to_base_10(num, base, &transfer, flag);
            if (transfer_status == ok_transfer)
            {
                if (stream == NULL) result += sprintf(buffer_ptr, "%lld", transfer);
                else result += fprintf(stream, "%lld", transfer);
            }
            else
            {
                print_errors(transfer_status, stream, &result, &buffer_ptr);
                break;
            }
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'i')
        {   
            format += 3; 
            int num = va_arg(*ptr, int);
            print_memory_dump(&num, INT, stream, &result, &buffer_ptr);
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'u')
        {   
            format += 3; 
            unsigned int num = va_arg(*ptr, unsigned int);
            print_memory_dump(&num, UINT, stream, &result, &buffer_ptr);
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'd')
        {   
            format += 3; 
            double num = va_arg(*ptr, double);
            print_memory_dump(&num, DOUBLE, stream, &result, &buffer_ptr);
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'f')
        {   
            format += 3; 
            float num = va_arg(*ptr, double);
            print_memory_dump(&num, FLOAT, stream, &result, &buffer_ptr);
        }
        else
        {
            if (stream == NULL) {result += sprintf(buffer_ptr, "%c", *format); buffer_ptr++;}
            else result += fputc(*format, stream); 
            format++;
        }
    }
    return result;
}



int overfprintf(FILE* stream, const char* format, ...) {
    va_list ptr;
    va_start(ptr, format);
    int result = check_flags(stream, format, &ptr, NULL);
    va_end(ptr);
    return result;
}

int oversprintf(char** str, const char* format, ...) {
    va_list ptr;
    va_start(ptr, format);
    int result = check_flags(NULL, format, &ptr, str);
    va_end(ptr);
    return result;
}