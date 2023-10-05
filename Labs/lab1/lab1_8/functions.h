#ifndef FUNCTION_H
#define FUNCTION_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

#define SIZE 33



//info print
void print_help(char * programm_name) {
    printf("Use %s <input_file> <output_file>\n", programm_name);
    printf("-help: Display this help message.\n");
    printf("-info: Display info message.\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_8\n\n");
}
//-----------------

int char_to_num(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return c - 'A' + 10;
    }
    return -1; 
}


long long ss_to_base_10(char* str, int base) {
    long long result = 0;
    int len = strlen(str);
    int i = len - 1;
    long long multiplier = 1;
    for (; i >= 0; i--) {
        int digit = char_to_num(str[i]);
        result = result + digit * multiplier;
        multiplier *= base;
    }
    return result;
}



enum is_valid_number_status_codes
{
    valid,
    not_valid
};


enum is_valid_number_status_codes check_symbol(char c) {
    if (isdigit(c) || (isalpha(c) && c >= 'A' && c <= 'Z')) {
        return valid; 
    }
    return not_valid; 
}

int find_base(char c)
{   
    if (c >= '0' && c <= '1') return 1;
    if (c >= '2' && c <= '9')
    {
        return c - '0' + 1;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return c - 'A' + 10 + 1;
    } 
    return 0;
}


//Checkers
enum input_check_status_codes
{
    invalid_input, 
    file_error,
    info_input_help,
    info_input,
    true_input
};

enum input_check_status_codes Input_checker(int argc, char* argv[], FILE ** input_file)
{
    if (argc != 3)
    {
        if (argc == 2 && !(strcmp(argv[1], "-help"))) {
            return info_input_help;
        }
        else if (argc == 2 && !(strcmp(argv[1], "-info"))) {
            return info_input;
        }
        return invalid_input;
    }
    *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        return file_error;
    }
    else
    {
        return true_input;
    }
    return invalid_input;
} 


//---------------






#endif