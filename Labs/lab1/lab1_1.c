#include "function.h"


enum digit_check_status_codes
{
    overflow,
    underflow,
    digit,
    not_digit
};

enum digit_check_status_codes is_digit(const char *str, long long *value_from_string) {
    char *endptr;
    *value_from_string = strtoll(str, &endptr, 10); 
    if (*str != '\0' && *endptr == '\0')
    {
        if (*value_from_string >= LLONG_MAX)
        {
            return overflow;
        }
        else if (*value_from_string <= LLONG_MIN)
        {
            return underflow;
        }
        return digit;
    }
    value_from_string = NULL;
    return not_digit;
}

bool is_negative(char* number)
{
    if (number[0] == '-')
    {
        return true;
    }
    return false;
}

bool is_negative_null(char ** number)
{
    if (is_negative(*number) && *number[1] == '0')
    {
        number++;
        return true;
    }
    return false;
}








enum flag_check_status_codes
{
    invalid_flag,
    undefined_flag,
    true_flag
};

enum flag_check_status_codes flag_checker(char* flag)
{
    if (flag[0] != '-')
    {
        return invalid_flag;
    }
    else if (flag[1] != 'h' && flag[1] != 'p' && 
                        flag[1] != 's' && flag[1] != 'e' &&
                                flag[1] != 'a' && flag[1] != 'f')
    {
        return undefined_flag;
    }
    return true_flag;
}

enum input_check_status_codes
{
    overflow_input,
    underflow_intput,
    invalid_input, 
    invalid_input_flag, 
    undefined_input_flag,
    info_input_help,
    info_input,
    true_input
};

enum input_check_status_codes Input_checker(int argc, char* argv[], long long * number, char** str_number, char* flag)
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
    switch (is_digit(argv[1], number))
    {
        case digit: 
            *str_number = argv[1]; 
            switch(flag_checker(argv[2]))
            {
                case invalid_flag:
                    return invalid_input_flag;
                case undefined_flag:
                    return undefined_input_flag;
                case true_flag:
                    *flag = argv[2][1];
                    return true_input;
            }
        case not_digit:
            return invalid_input;
        case overflow:
            return overflow_input;
        case underflow:
            return underflow_intput;
    }
    return invalid_input;
} 

void use_flag(long long * number, char ** str_number, char flag, unsigned char ** array_of_multiples)
{
    switch (flag) 
        {
        case 'h':
            switch(find_multiples(*number, &array_of_multiples))
            {
                case has_multiples:
                    print_multiples(*number, *array_of_multiples);
                    break;
                case not_multiples:
                    printf("The number %lld has no multiples\n", *number);
                    break;
            }
            break;
        case 'p':
            if (is_prime(*number)) 
            {
                printf("%s is prime number\n", *str_number);
            } else 
            {
                printf("%s is composite number\n", *str_number);
            }
            break;
        // case 's':
        //     print_digits(number);
        //     break;
        // case 'a':
        //     printf("Sum of natural numbers from 1 to %s is %d\n", number, sum_of_natural_numbers(atoi(number)));
        //     break;
        // case 'f':
        //     unsigned long result;
        //     switch(factorial(number, &result))
        //     {
        //         case fsc_ok: 
        //             printf("%s! = %lu\n", number, result); 
        //             break; 
        //         case fsc_overflow: 
        //             printf("Overflow detected!"); 
        //             break; 
        //         case fsc_invalid_parameter: 
        //             printf("Invalid parameter detected!"); 
        //             break; 
        //     }
        //     break;
        // case 'e':
        //     table_of_degrees(number);
        //     break;
        // default:
        //     printf("Invalid flag: %c\n", flag);
        //     break;
    }
}




void print_help() {
    printf("Choose an action:\n");
    printf("-h: Print natural numbers within 100 that are multiples of the specified number.\n");
    printf("-p: Check if a number is prime or composite.\n");
    printf("-s: Divide the number into individual digits and print them.\n");
    printf("-a: Calculate the sum of natural numbers from 1 to the specified number.\n");
    printf("-f: Calculate the factorial of the specified number.\n");
    printf("-help: Display this help message.\n\n");
}

void print_info() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("\nM8O-211Б-22\n");
    printf("Laboratory work #1_1\n\n");
}

int main(int argc, char* argv[])
{

    long long number;
    unsigned long * result;
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
        printf("True input\n");
        use_flag(&number, &str_number, flag, &array);
        break;
    }

    is_negative_null(&str_number);
    printf("\n\n%lld\n", number);
    printf("\n\n%lld\n", LLONG_MAX);
    printf("%s\n", str_number);
    printf("%c\n", flag);


    free(array);


    

return 0;


}