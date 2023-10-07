#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


#define SIZE 100
#define SIZE_TRANSFER 33

void print_help(char * programm_name) {
    printf("Use %s <-r> <input_file1> <input_file2> <result_file>  OR   Use %s <-a> <input_file> <output_file>\n", programm_name, programm_name);
    printf("-help: Display this help message.\n");
    printf("-info: Display info message.\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_7\n\n");
}



//moves pointer to first non empty
void skip_trashes(FILE *file)
{
    char symbol;
    while ((symbol = fgetc(file)) != EOF) 
    {
        if (symbol != '\n' && symbol != ' ' && symbol != '\t')  
        { 
            ungetc(symbol, file); //returns back to the stream
            break; 
        }
    }
}

bool is_latin_ch(char symbol) 
{
    return isalpha(symbol) && ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z'));
}


void str_tolower(char* str)
{
    unsigned int len = strlen(str);
    for (int i = 0; i < len; i++) 
    {
        if(is_latin_ch(str[i])) 
        {
            str[i] = tolower(str[i]);
        }
    }
}


//flag r
enum check_with_read
{
    well_read,
    long_string
};

char* get_str(FILE *file, enum check_with_read* status)
{
    char *str = (char *)malloc(sizeof(char) * SIZE);
    char symbol;
    unsigned int i = 0;
    *status = well_read;

    while ((symbol = fgetc(file)) != EOF) 
    {
        if (symbol == '\n' || symbol == ' ' || symbol == '\t') 
        { 
            break; 
        }
        if (i < SIZE) 
        {
            str[i] = symbol;
            i++;
        }
        else
        { 
            *status = long_string; 
            return NULL; 
        }
    }
    str[i] = '\0';
    return str;
}

enum check_with_read read_write_files_r(FILE *file1, FILE *file2, FILE *result)
{
    bool flag = true;
    char *str;
    skip_trashes(file1); skip_trashes(file2);   

    while (!feof(file1) || !feof(file2)) 
    {
        enum check_with_read status;

        if (flag)
        {
            if (feof(file1)) //if the first file is over
            {
                flag = false; 
                continue; 
            }
            str = get_str(file1, &status);
            if (status == long_string) 
            {
                return long_string; 
            }
            skip_trashes(file1);
            flag = false;
        }
        else
        {
            if (feof(file2)) 
            {
                flag = true; 
                continue; 
            }
            str = get_str(file2, &status);
            if (status == long_string) 
            {
                return long_string; 
            }
            skip_trashes(file2);
            flag = true;
        }
        fprintf(result, "%s ", str);
        free(str);
    }
    return well_read;
}

enum r_flag_status_codes
{
    r_file_error,
    r_correct,
    r_invalid_string
};

enum r_flag_status_codes flag_r(char *path_to_file1, char *path_to_file2, char *result_path)
{
    FILE *file1 = NULL, *file2 = NULL, *result = NULL;

    if(!(file1 = fopen(path_to_file1, "r")) || !(file2 = fopen(path_to_file2, "r")) || !(result = fopen(result_path, "w")))
    {
        return r_file_error;
    }
    enum check_with_read status = read_write_files_r(file1, file2, result);
    if (fclose(file1) != 0 || fclose(file2) != 0 || fclose(result) != 0) 
    {
        return r_file_error;
    } 
    switch (status)
    {
        case well_read:
            return r_correct;
        case long_string:
            return r_invalid_string;
    } 
}




//flag a
void transfer_to_crnt_base(char * ptr, int * counter, int symbol, int base)
{
    int r;
    *ptr = 0;
    ptr--;
    while (symbol > 0) {
        r = symbol % base;
        *ptr = (r > 9) ? r - 10 + 'A' : r + 48;
        ptr--;
        symbol /= base;
        (*counter)++;
    }
}

void to_ascii_and_4base(FILE *file, char* str, int base)
{
    for (int i = 0; i < strlen(str); i++) 
    {
        int num = (int)str[i];
        char buffer[SIZE];
        char *ptr = buffer + SIZE - 1;
        int counter = 0;
        transfer_to_crnt_base(ptr, &counter, num, base);
        for (int i = SIZE - counter - 1; i < SIZE - 1; i++) 
        {
            fprintf(file, "%c", buffer[i]);
        }
    }
}

enum a_flag_status_codes
{
    a_file_error,
    a_invalid_string,
    a_correct
};


enum check_with_read read_write_files_a(FILE *file, FILE *result)
{
    skip_trashes(file);
    char *str;
    bool flag = true;
    unsigned int crnt_str = 1;

    while (!feof(file))
    {
        enum check_with_read status;
        str = get_str(file, &status);
        if (status == long_string)
        {
            return long_string;
        }
        if (crnt_str % 10 == 0)
        {
            str_tolower(str);
            to_ascii_and_4base(result, str, 4);
            fprintf(result, " ");
        }
        else if (crnt_str % 2 == 0)
        {
            char *endptr;
            str_tolower(str);
            fprintf(result, "%s ", str);
        }
        else if (crnt_str % 5 == 0)
        {
            to_ascii_and_4base(result, str, 8);
            fprintf(result, " ");
        }
        else 
        { 
            fprintf(result, "%s ", str); 
        }
        crnt_str++;
        free(str);
        skip_trashes(file);
    }
    return well_read;
}


enum a_flag_status_codes flag_a(char *path_to_file1, char *path_to_file2)
{
    FILE *file = NULL, *result = NULL;

    if(!(file = fopen(path_to_file1, "r")) || !(result = fopen(path_to_file2, "w")))
    {
        return a_file_error;
    }

    enum check_with_read status = read_write_files_a(file, result);
    if (fclose(file) != 0 || fclose(result) != 0) 
    {
        return a_file_error;
    } 
    switch (status)
    {
        case well_read:
            return a_correct;
        case long_string:
            return a_invalid_string;
    } 
}

//Checkers
enum input_check_status_codes
{
    invalid_input,
    errors_with_file_contents, 
    error_with_files,
    info_input_help,
    info_input,
    true_input
};

enum input_check_status_codes input_checker(int argc, char* argv[], char * flag)
{
    if (argc < 3)
    {
        if (argc == 2 && !(strcmp(argv[1], "-help"))) {
            return info_input_help;
        }
        else if (argc == 2 && !(strcmp(argv[1], "-info"))) {
            return info_input;
        }
        return invalid_input;
    }
    if (strlen(argv[1]) == 2 && (argv[1][0] == '-' || argv[1][0] == '/') 
                && (argv[1][1] == 'r' || argv[1][1] == 'a')) 
    {
        *flag = argv[1][1];

        switch (*flag)
        {
            case 'r':
                if (argc == 5)
                {
                    switch(flag_r(argv[2], argv[3], argv[4]))
                    {
                        case r_correct:
                            return true_input;
                        case r_file_error:
                            return error_with_files;
                        case r_invalid_string:
                            return errors_with_file_contents;
                    }
                }
                break;
            case 'a':
                if (argc == 4)
                {
                    switch(flag_a(argv[2], argv[3]))
                    {
                        case a_correct:
                            return true_input;
                        case a_file_error:
                            return error_with_files;
                        case a_invalid_string:
                            return errors_with_file_contents;
                    }
                }
                break;
            default:
                break;
        } 
    }
    return invalid_input;
} 


//---------------

#endif