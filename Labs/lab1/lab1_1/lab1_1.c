#include "function.h"



int main(int argc, char* argv[])
{
    long long number;
    long long result;
    char* str_number;
    char flag;
    unsigned char * array = NULL;


    switch (Input_checker(argc, argv, &number, &str_number, &flag))
    {
    case invalid_input:
        printf("\nInvalid input\n");
        printf("Use: %s <number> <flag> (use %s -help to list all functions)\n\n", argv[0], argv[0]);
        return 1;
    case info_input:
        print_info();
        break;
    case info_input_help:
        print_help();
        break;
    case invalid_input_flag:
        printf("Invalid flag\n");
        return 1;
    case undefined_input_flag:
        printf("Undefined flag\n");
        return 1;
    case overflow_input:
        printf("Overflow_input\n");
        return 1;
    case underflow_intput:
        printf("Underflow_intput\n");
        return 1;
    case true_input:
        is_negative_null(&str_number, number);
        use_flag(&number, &str_number, flag, &array, &result);
        break;
    }


return 0;


}