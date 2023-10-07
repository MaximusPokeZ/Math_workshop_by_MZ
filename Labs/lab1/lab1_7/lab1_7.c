#include "functions.h"




int main(int argc, char *argv[])
{
    char flag;
    switch (input_checker(argc, argv, &flag))
    {
    case invalid_input:
        printf("Error: invalid input\n");
        break;
     case error_with_files:
        printf("Error: there is a problem with the function, please check the files\n");
        break;
    case errors_with_file_contents:
        printf("Error: There are problems with the contents of the files\n");
        break;
    case info_input:
        print_info();
        break;
    case info_input_help:
        print_help(argv[0]);
        break;
    case true_input:
        printf("OK!\n");
        break;
    default:
        break;
    }
    
    return 0;
}