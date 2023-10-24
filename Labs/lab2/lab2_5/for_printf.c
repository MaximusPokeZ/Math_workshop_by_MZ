#include "functions.c"

#define STRING_SIZE 300
#define FLAG_SIZE 20

void print_errors (enum transfer_to_status_codes status)
{
    if (status == out_of_range)
    {
        printf("Number out of range for Roman numerals\n");
    }
    else if (status == memory_allocation_problem)
    {
        printf("Memory allocation problem\n");
    }
    else 
    {
        printf("Unknown data type\n");
    }
}


int process_user_flags(const char* format, va_list* ptr, char* result_buffer, size_t buffer_size)
{
    int chars_written = 0;
    int for_n = 0;
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
                int chars_to_write = snprintf(result_buffer + chars_written, buffer_size - chars_written, "%s", roman);
                if (chars_to_write < 0)
                {
                    return -1;
                }
                chars_written += chars_to_write;
                free(roman);
            }
            else
            {
                print_errors(roman_status);
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
                int chars_to_write = snprintf(result_buffer + chars_written, buffer_size - chars_written, "%s", cyckendorf);
                if (chars_to_write < 0)
                {
                    return -1;
                }
                chars_written += chars_to_write;
                free(cyckendorf);
            }
            else
            {
                print_errors(cyckendorf_status);
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
                    int chars_to_write = snprintf(result_buffer + chars_written, buffer_size - chars_written, "%c", transfer[i]);
                    chars_written += chars_to_write;
                }
                free(transfer);
            }
            else
            {
                print_errors(transfer_status);
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
                int chars_to_write = snprintf(result_buffer + chars_written, buffer_size - chars_written, "%lld", transfer);
                if (chars_to_write < 0)
                {
                    return -1;
                }
                chars_written += chars_to_write;
            }
            else
            {
                print_errors(transfer_status);
            }
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'i')
        {   
            format += 3; 
            int num = va_arg(*ptr, int);
            int chars_to_write = chars_written;
            enum transfer_to_status_codes dump_status = print_memory_dump(&num, INT, result_buffer, &chars_written, buffer_size);
            if (dump_status == ok_transfer)
            {
                if (chars_to_write > chars_written)
                {
                    return -1;
                }
            }
            else
            {
                print_errors(dump_status);
            }
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'u')
        {   
            format += 3; 
            unsigned int num = va_arg(*ptr, unsigned int);
            int chars_to_write = chars_written;
            enum transfer_to_status_codes dump_status = print_memory_dump(&num, UINT, result_buffer, &chars_written, buffer_size);
            if (dump_status == ok_transfer)
            {
                if (chars_to_write > chars_written)
                {
                    return -1;
                }
            }
            else
            {
                print_errors(dump_status);
            }
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'd')
        {   
            format += 3; 
            double num = va_arg(*ptr, double);
            int chars_to_write = chars_written;
            enum transfer_to_status_codes dump_status = print_memory_dump(&num, DOUBLE, result_buffer, &chars_written, buffer_size);
            if (dump_status == ok_transfer)
            {
                if (chars_to_write > chars_written)
                {
                    return -1;
                }
            }
            else
            {
                print_errors(dump_status);
            }
        }
        else if (*format == '%' && *(format + 1) == 'm' && *(format + 2) == 'f')
        {   
            format += 3; 
            float num = va_arg(*ptr, double);
            int chars_to_write = chars_written;
            enum transfer_to_status_codes dump_status = print_memory_dump(&num, FLOAT, result_buffer, &chars_written, buffer_size);
            if (dump_status == ok_transfer)
            {
                if (chars_to_write > chars_written)
                {
                    return -1;
                }
            }
            else
            {
                print_errors(dump_status);
            }
        }
        else if (*format == '%' && *(format + 1) == 'n')
        {
            format += 2;
            int* n_ptr = va_arg(*ptr, int*);
            int chars_to_write;
            int minus_prev = for_n;
            if (n_ptr != NULL)
            {
                chars_to_write = vsnprintf(result_buffer, buffer_size, result_buffer, *ptr);
                for_n += chars_to_write;
                if (chars_to_write < 0)
                {
                    return -1;
                }
                *n_ptr = chars_to_write - minus_prev;
            }
        }
        else if (*format == '%')
        {
            char flag[FLAG_SIZE];
            int i = 0;
            flag[i] = *format; i++;
            format++;
            while (*format != ' ' && *format != '\t' && *format != '\n' && *format != '%' && *format != 'd' && *format != 'f' && *format != 'n')
            {
                flag[i] = *format;
                format++; i++;
            }
            flag[i] = *format; i++; flag[i] = '\0';
            format++;
            chars_written += vsprintf(result_buffer + chars_written, flag, *ptr);
        }
        else
        {
            int chars_to_write = snprintf(result_buffer + chars_written, buffer_size - chars_written, "%c", *format);
            if (chars_to_write < 0) 
            {
                return -1;
            }
            chars_written += chars_to_write;
            format++;
        }
    }
    return chars_written;
}



int overfprintf(FILE* stream, const char* format, ...) 
{
    va_list ptr, ptr_c;
    va_start(ptr, format);
    va_copy(ptr_c, ptr);
    

    char result_buffer[STRING_SIZE];
    int user_flags_length = process_user_flags(format, &ptr, result_buffer, sizeof(result_buffer));
    
    int result = 0;

    if (user_flags_length >= 0)
    {
        result += fprintf(stream, "%s", result_buffer);
    }

    va_end(ptr);
    return result;
}

int oversprintf(char** str, const char* format, ...) {
    va_list ptr;
    va_start(ptr, format);

    char result_buffer[STRING_SIZE];
    int user_flags_length = process_user_flags(format, &ptr, result_buffer, sizeof(result_buffer));
    
    int result = 0;

    if (user_flags_length >= 0)
    {
        result += sprintf(*str, "%s", result_buffer);
    }
    (*str)[result] = '\0';

    va_end(ptr);
    return result;
}
