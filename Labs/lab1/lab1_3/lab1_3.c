#include "function.h"



int main(int argc, char* argv[])
{
    long double parameters[4];
    long double result;
    char flag;


    switch (Input_checker(argc, argv, parameters, &flag))
    {
    case invalid_input:
        printf("\nInvalid input\n");
        printf("Use: %s <number> <flag> (use %s -help to list all functions)\n\n", argv[0], argv[0]);
        break;
    case info_input:
        print_info();
        break;
    case info_input_help:
        print_help();
        break;
    case invalid_input_flag:
        printf("Invalid flag\n");
        break;
    case undefined_input_flag:
        printf("Undefined flag\n");
        break;
    case overflow_input:
        printf("Overflow_input\n");
        break;
    case underflow_intput:
        printf("Underflow_intput\n");
        break;
    case true_input:
        use_flag(flag, parameters, argc);
        break;
    }


return 0;


}