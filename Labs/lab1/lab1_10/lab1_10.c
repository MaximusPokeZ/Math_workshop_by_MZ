#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define SIZE 33


enum start_status_codes
{
    good_enter_radix,
    not_good_radix
};

enum start_status_codes check_start_enter (int radix)
{
    if (radix < 2 || radix > 36)
    {
        return not_good_radix;
    }
    return good_enter_radix;
}

// Функция для перевода символа в число (0-9, A-Z)
int char_to_num(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
     else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return -1; // Недопустимый символ
}

enum correct_input_status_codes
{
    correct,
    not_correct
};

enum transfer_from_str
{   
    ok_transfer,
    invalid_character
};


// Функция для перевода строки в целое число с заданным основанием
enum transfer_from_str ss_to_base_10(char* str, int base, long long * result) {
    int len = strlen(str);
    int i = len - 1;
    for (; i > 0; i--) {
        int digit = char_to_num(str[i]);
        if (digit == -1 || digit >= base) {
            return invalid_character;
        }
        *result = *result * base + digit;
    }
    if (str[i] == '-')
    {
        *result = -(*result);
    }
    else
    {
        int digit = char_to_num(str[i]);
        if (digit == -1 || digit >= base) {
            return invalid_character;
        }
        *result = *result * base + digit;
    }
    return ok_transfer;
}



int main()
{
    int radix;
    printf("Enter the radix of the number system into the console (in the range [2..36]): ");
    scanf("%d", &radix);
    switch(check_start_enter(radix))
    {
        case good_enter_radix:
            break;
        case not_good_radix:
            printf("Your radix is not within the specified range\n");
            return 0;
    }

    char input[SIZE];
    long long max_number, temp = 0;
    enum correct_input_status_codes flag = correct;
    printf("Enter numbers in systems with yor radix or Stop:\n");
    scanf("%s", input);


    if(strcmp(input, "Stop") == 0)
    {
        flag = not_correct;
    }
    else
    {
        switch (ss_to_base_10(input, radix, &temp))
        {
            case invalid_character:
                printf("The number contains an invalid character\n");
                break;
            case ok_transfer:
                flag = correct;
                max_number = temp;
        }
        while (1)
        {  
            temp = 0;
            scanf("%s", input);
            if(strcmp(input, "Stop") == 0)
            {
                break;
            }
            switch (ss_to_base_10(input, radix, &temp))
            {
                case invalid_character:
                    printf("The number contains an invalid character\n");
                    break;
                case ok_transfer:
                    if (max_number < temp)
                    {
                        max_number = temp;
                    }
            }
        }
    }
    
    

    switch (flag) 
    {
        case correct:
            printf("The maximum of your numbers: %lld\n", max_number);
            break;
        case not_correct:
            printf("The numbers were not entered or they are incorrect\n");
            break;
    }

    return 0;
}

