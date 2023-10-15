#include "functions.c"

int main(int argc, char *argv[])
{
    char flag;
    switch(check_input(argc, argv, &flag))
    {
        case invalid_input:
            printf("Error: invalid input!\n");
            return 0;
        case ok_input:
            break;
    }
    if (flag == 'l')
    {
        size_t length;
        switch(string_length(argv[2], &length))
        {
            case error_argum:
                printf("Error with string argument!\n");
                break;
            case ok_argum:
                printf("Length of string <%s> is : %zu\n", argv[2], length);
                break;
            default: break;
        }
    }
    else if (flag == 'r')
    {
        char *result_str;
        switch(reverse(argv[2], &result_str))
        {
            case error_alloc_memo:
                printf("Memory allocation problem\n");
                break;
            case error_argum:
                printf("Error with string argument!\n");
                break;
            case ok_argum:
                printf("Reversed string %s is : %s\n", argv[2], result_str);
                break;
        }
        free(result_str);
    }
    else if (flag == 'u')
    {
        char *result_str;
        switch(convert_flag_u(argv[2], &result_str))
        {
            case error_alloc_memo:
                printf("Memory allocation problem\n");
                break;
            case error_argum:
                printf("Error with string argument!\n");
                break;
            case ok_argum:
                printf("Converting with flag -u string %s to string %s\n", argv[2], result_str);
                break;
        }
        free(result_str);
    }
    else if (flag == 'n')
    {
        char *result_str;
        switch(convert_flag_n(argv[2], &result_str))
        {
            case error_alloc_memo:
                printf("Memory allocation problem\n");
                break;
            case error_argum:
                printf("Error with string argument!\n");
                break;
            case ok_argum:
                printf("Converting with flag -n string %s to string %s\n", argv[2], result_str);
                break;
        }
        free(result_str);
    }
    else if (flag == 'c' && argc >= 4)
    {
        unsigned int seed;
        switch(string_to_unsigned_int(argv[3], &seed))
        {
            case overflow:
                printf("The number is overflow!\n");
                return 0;
            case not_number:
                printf("It is not number\n");
                return 0;
            case ok_number:
                break;
        }
        srand(seed);
        char **strings = (char **)malloc(sizeof(char *) * (argc - 3));
        if (strings == NULL)
        {
            printf("Memory allocation problem\n");
            return 0;
        }
        strings[0] = argv[2];
        for (int i = 1; i < argc - 3; i++)
        {
            strings[i] = argv[3 + i];
        }
        char *result_str;
        switch(convert_flag_c(strings, argc - 3, &result_str))
        {
            case error_alloc_memo:
                printf("Memory allocation problem\n");
                break;
            case error_argum:
                printf("Error with string argument!\n");
                break;
            case ok_argum:
                printf("Converting with flag -c : string %s\n", result_str);
                break;
        }
        free(result_str);
        free(strings);
    }
    else
    {
        printf("Error: invalid input!\n");
    }
    return 0;
}