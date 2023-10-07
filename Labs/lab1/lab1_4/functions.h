#ifndef FUNCTION_H
#define FUNCTION_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 33

void print_help(char * programm_name) {
    printf("Use %s <flag> <input_file>   OR   Use %s <nflag> <input_file> <output_file>\n", programm_name, programm_name);
    printf("-help: Display this help message.\n");
    printf("-info: Display info message.\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_4\n\n");
}

char * create_output_file(char *input_file_name) {
    int length = strlen(input_file_name);
    int out_length = length;
    char *output_file_name = (char*)malloc(sizeof(char) * out_length * 2);
    if(output_file_name == NULL)
    {
        return NULL;
    }
    while (input_file_name[length] != '/' && length != 0) 
    {
        length--;
    }

    if (length == 0) 
    {
        output_file_name[length] = 'o';
        output_file_name[length + 1] = 'u';
        output_file_name[length + 2] = 't';
        output_file_name[length + 3] = '_';
        for (int i = length + 4; i < out_length + 4; i++) 
        {
            output_file_name[i] = input_file_name[i - 4];
        }
        output_file_name[out_length + 4] = '\0';
    } 
    else 
    {
        for (int i = 0; i < length + 1; i++) 
        {
            output_file_name[i] = input_file_name[i];
        }
        output_file_name[length + 1] = 'o';
        output_file_name[length + 2] = 'u';
        output_file_name[length + 3] = 't';
        output_file_name[length + 4] = '_';
        for (int i = length + 5; i < out_length + 4; i++) 
        {
            output_file_name[i] = input_file_name[i - 4];
        }
        output_file_name[out_length + 4] = '\0';
    }
    return output_file_name;
}

enum flag_check_and_n
{
    is_n,
    not_n,
    undefined_flag,
    not_flag
};

enum flag_check_and_n check_flags(char * flag)
{
    if (flag[0] == '-'|| flag[0] == '/')
    {
        if (flag[1] == 'd' || flag[1] == 'i' || flag[1] == 's' || flag[1] == 'a')
        {
            return not_n;
        }
        if (flag[1] == 'n' &&  (flag[2] == 'd' || flag[2] == 'i' || flag[2] == 's' || flag[2] == 'a'))
        {
            return is_n;
        }
        return undefined_flag;
    }
    return not_flag;
}



enum input_status_code {
    info_input_help,
    info_input,
    invalid_input,
    flag_d,
    flag_i,
    flag_s,
    flag_a,
};


enum input_status_code input_checker(int argc, char **argv, char ** out_name) {
    if (argc < 3 || argc > 4) 
    {
        return invalid_input;
    } 
    else if (!strcmp(argv[1], "-help") || !strcmp(argv[1], "/help")) 
    {
        return info_input_help;
    } 
    else if (!strcmp(argv[1], "-info") || !strcmp(argv[1], "/info")) 
    {
        return info_input;
    }
    else if (argc == 3 || argc == 4) 
    {
        char flag;
        switch (check_flags(argv[1]))
        {
            case is_n:
                *out_name = argv[3];
                flag = argv[1][2];
                break;
            case not_n:
                *out_name = create_output_file(argv[2]);
                flag = argv[1][1];
                break;
            case undefined_flag:
                return invalid_input;
            case not_flag:
                return invalid_input;
        }
        switch (flag)
        {
            case 'd':
                return flag_d;
            case 'i':
                return flag_i;
            case 's':
                return flag_s;
            case 'a':
                return flag_a; 
        }
    }
    return invalid_input;
}

//File open checkers
enum is_open_file_read_write {
    file_error,
    file_ok
};


enum is_open_file_read_write file_read_write(char *file_name, char * letter) {
    FILE *file = NULL;
    file = fopen(file_name, letter);
    if (!(file)) 
    {
        return file_error;
    } 
    else 
    {
        fclose(file);
        return file_ok;
    }
}

//----------------------------



//tasks
void without_arabic(char *input_file_name, char* output_file_name) 
{
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    if (!(input_file = fopen(input_file_name, "r")) || !(output_file = fopen(output_file_name, "w"))) 
    {
        printf("Error with openning files!\n");
    } 
    else 
    {
        int symbol;
        while ((symbol = fgetc(input_file)) != EOF) 
        {
            if (symbol >= 48 && symbol <= 57) 
            {
                continue;
            }
            fprintf(output_file, "%c", symbol);
        }
    }
    if (fclose(output_file) != 0 || fclose(input_file) != 0) 
    {
        printf("An error occurred while closing the file\n");
    } 
}


void count_of_latin_or_not_latin (char *input_file_name, char* output_file_name, char flag_latin_not)
{
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    if (!(input_file = fopen(input_file_name, "r")) || !(output_file = fopen(output_file_name, "w"))) 
    {
        printf("Error with openning files!\n");
    } 
    else 
    {
        unsigned long number_of_str = 1, counter = 0;
        char symbol;
        switch (flag_latin_not)
        {
            case 'l':
                while ((symbol = fgetc(input_file)) != EOF)
                {
                    if (symbol == '\n')
                    {
                        fprintf(output_file, "%lu line:  %lu\n", number_of_str, counter);
                        number_of_str++;
                        counter = 0;
                    }
                    else if ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z')) 
                    {
                        counter++;
                    }
                }
                break;

            case 'n':
                while ((symbol = fgetc(input_file)) != EOF)
                {   
                    if (symbol == '\n')
                    {
                        fprintf(output_file, "%lu line:  %lu\n", number_of_str, counter);
                        number_of_str++;
                        counter = 0;
                    }
                    else if (symbol != ' ' && (symbol < 'A' || symbol > 'Z') && (symbol < 'a' || symbol > 'z') && (symbol < '0' || symbol > '9')) 
                    {
                        counter++;
                    }
                }
                break;
        }
        fprintf(output_file, "%lu line:  %lu\n", number_of_str, counter);
    }
    if (fclose(output_file) != 0 || fclose(input_file) != 0) 
    {
        printf("An error occurred while closing the file\n");
    } 
}


void transfer_to16 (char * ptr, int * counter, int symbol)
{
    int r;
    int base = 16;
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

void liters_to_16ss (char *input_file_name, char* output_file_name, char status)
{
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    if (!(input_file = fopen(input_file_name, "r")) || !(output_file = fopen(output_file_name, "w"))) 
    {
        printf("Error with openning files!\n");
    } 
    else 
    {
        char symbol;
        while ((symbol = fgetc(input_file)) != EOF) {
            if (symbol >= 48 && symbol <= 57) 
            {
                fprintf(output_file, "%c", symbol);
                continue;
            } 
            else 
            {
                char buffer[SIZE];
                char *ptr = buffer + SIZE - 1;
                int counter = 0;
                transfer_to16(ptr, &counter, (int)symbol);
                for (int i = SIZE - counter - 1; i < SIZE - 1; i++) {
                    fprintf(output_file, "%c", buffer[i]);
                }
            }
        }
    }
    if (fclose(output_file) != 0 || fclose(input_file) != 0) 
    {
        printf("An error occurred while closing the file\n");
    } 
}


#endif