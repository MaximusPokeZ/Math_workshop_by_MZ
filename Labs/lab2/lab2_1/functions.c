#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>



bool _strcmp(const char *str_1, const char *str_2)
{
    while (*str_1 != '\0' && *str_2 != '\0')
    {
        if (*str_1 != *str_2) return false;
        str_1++;
        str_2++;
    }
    if (*str_1 != '\0' || *str_2 != '\0') return false;
    return true;
}

void swap(char **s1, char **s2)
{
    char *temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}


bool is_alpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool is_digit(char c)
{
    return ('0' <= c && c <= '9');
}

char toapper(char c)
{
    return ('a' <= c && c <= 'z') ? c - 'a' + 'A' : c;
}

bool is_number (char * str, unsigned int * number)
{
    while (str)
    {
        if (!is_digit(*str)) return false;
        str++;
    }
    return true;
    
}

enum string_to_number
{
    overflow,
    not_number,
    ok_number
};

enum string_to_number string_to_unsigned_int(const char* str, unsigned int * result) 
{
    *result = 0;
    int i = 0;
    while (str[i] != '\0') 
    {
        if (is_digit(str[i])) 
        {
            if (*result > (unsigned int)-1 / 10) 
            {
                return overflow;
            }
            *result = *result * 10 + (str[i] - '0');
        } 
        else return not_number;
        i++;
    }
    return ok_number;
}

enum task_checker
{
    ok_argum,
    error_argum, 
    error_alloc_memo
};

enum task_checker string_length(char *string, size_t *len)
{
    *len = 0;
    if (string == NULL)
    {
        return error_argum;
    }
    const char * end = string;
    for (; *end != '\0'; ++end)
    {
        (*len)++;
    }
    return ok_argum;
}

enum task_checker reverse(char *string, char **result)
{
    size_t len;
    switch(string_length(string, &len))
    {
        case error_argum:
            return error_argum;
        default:
            break;
    }

    *result = (char *)malloc(sizeof(char) * (len + 1));
    if (*result == NULL)
    {
        return error_alloc_memo;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        (*result)[len - 1 - i] = string[i];
    }
    (*result)[len] = '\0';

    return ok_argum;
}

enum task_checker convert_flag_u(char *string, char **result)
{
    size_t len;
    switch(string_length(string, &len))
    {
        case error_argum:
            return error_argum;
        default:
            break;
    }
    *result = (char *)malloc(sizeof(char) * (len + 1));
    if (*result == NULL)
    {
        return error_alloc_memo;
    }
    for (int i = 0; i < len; i++)
    {
        if (i % 2) {
            (*result)[i] = string[i];
        }
        else (*result)[i] = toapper(string[i]);
    }
    (*result)[len] = '\0';

    return ok_argum;
}

enum task_checker convert_flag_n(char *string, char **result)
{
    size_t len;
    switch(string_length(string, &len))
    {
        case error_argum:
            return error_argum;
        default:
            break;
    }
    *result = (char *)malloc(sizeof(char) * (len + 1));
    if (*result == NULL)
    {
        return error_alloc_memo;
    }

    unsigned int сount = 0;
    for (int i = 0; i < len; i++)
    {
        if (is_digit(string[i]))
        {
            (*result)[сount++] = string[i];
        }
    }
    for (int i = 0; i < len; i++)
    {
        if (is_alpha(string[i]))
        {
            (*result)[сount++] = string[i];
        }
    }
    for (int i = 0; i < len; i++)
    {
        if (!is_alpha(string[i]) && !is_digit(string[i]))
        {
            (*result)[сount++] = string[i];
        }
    }
    (*result)[len] = '\0';
    return ok_argum;
}

enum task_checker convert_flag_c(char *strings[], unsigned int len, char **result)
{
    if (strings == NULL)
    {
        return error_argum;
    }

    size_t sumlen = 0;
    if (len > 1)
    {
        //mix
        for (int i = 0; i < len; i++)
        {
            int j = rand() % (len - 2);
            swap(&strings[i], &strings[j]);
        }
    }

    for (int i = 0; i < len; i++)
    {
        size_t crnt_len;
        switch(string_length(strings[i], &crnt_len))
        {
            case error_argum:
                return error_argum;
            case ok_argum:
                sumlen += crnt_len;
                break;
            default: break;
        }
    }
    *result = (char *)malloc(sizeof(char) * (sumlen + 1));
    if (*result == NULL)
    {
        return error_alloc_memo;
    }

    int crnt_string = 0;
    for (int i = 0; i < len; i++)
    {
        size_t crnt_len;
        switch(string_length(strings[i], &crnt_len))
        {
            case error_argum:
                return error_argum;
            case ok_argum:
                break;
            default: break;
        }

        for (size_t j = 0; j < crnt_len; j++)
        {
            (*result)[crnt_string++] = strings[i][j];
        }
    }
    return ok_argum;
}


//Checkers
enum input_check_status_codes
{
    invalid_input, 
    ok_input
};


enum input_check_status_codes check_input(int argc, char *argv[], char *flag)
{
    if (argc < 3)
    {
        return invalid_input;
    }
    if (argc == 3 || argc >= 4)
    {
        if (!_strcmp(argv[1], "-l") && !_strcmp(argv[1], "-r") && !_strcmp(argv[1], "-u") && !_strcmp(argv[1], "-n") && !_strcmp(argv[1], "-c"))
        {
            return invalid_input;
        }
        *flag = argv[1][1];
        return ok_input;
    }
    return invalid_input;
}

