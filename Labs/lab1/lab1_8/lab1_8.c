#include "functions.h"



int main(int argc, char* argv[])
{
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    char symbol;
    int base = 2, min_base = 2, i = 0;
    bool flag = false;
    char number[SIZE];
    char * number_ptr = number;
    long long decimal_value;
    


    switch (Input_checker(argc, argv, &input_file))
    {
    case invalid_input:
        printf("Invalid input\n");
        print_help(argv[0]);
        break;
    case info_input:
        print_info();
        break;
    case info_input_help:
        print_help(argv[0]);
        break;
    case file_error:
        printf("Error opening input file or file not found\n");
        break;
    case true_input:
        output_file = fopen(argv[2], "w");
        if (output_file == NULL) 
        {
            printf("Error opening output file or file not found\n");
            fclose(input_file); //?
            return 1;
        }
        while ((symbol = fgetc(input_file)) != EOF)
        {
            switch(check_symbol(symbol))
            {
                case not_valid:
                    if (flag)
                    {
                       number[i] = '\0';
                       decimal_value = ss_to_base_10(number_ptr, min_base);
                       fprintf(output_file, "%s %d %lld\n", number, min_base, decimal_value); 
                       min_base = 2;
                       i = 0;
                    }
                    flag = false;
                    break;
                case valid:
                    flag = true;
                    base = find_base(symbol);
                    if (base > min_base)
                    {
                        min_base = base;
                    }
                    if (i >= SIZE - 1)
                    {
                        printf("The number must be no longer than 32 characters\n");
                        continue;
                    }
                    number[i] = symbol;
                    i++;
                    break;
            }
        }
        break;
    }


    if (flag)
    {
        number[i] = '\0';
        decimal_value = ss_to_base_10(number_ptr, min_base);
        fprintf(output_file, "%s %d %lld\n", number, min_base, decimal_value); 
    }

    if (fclose(output_file) != 0 || fclose(input_file) != 0) 
    {
        printf("An error occurred while closing the file\n");
        return 1;
    } 

    return 0;


}