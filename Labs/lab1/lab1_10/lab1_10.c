#include "functions.h"

int main()
{
    int radix;
    printf("Enter the radix of the number system into the console (in the range [2..36]): ");
    scanf("%d", &radix);
    switch (check_start_enter(radix))
    {
    case good_enter_radix:
        break;
    case not_good_radix:
        printf("Your radix is not within the specified range\n");
        return 0;
    }

    char input[SIZE];
    long long max_number, temp = 0;
    enum correct_input_status_codes flag = not_correct;
    printf("Enter numbers in systems with your radix or Stop:\n");
    scanf("%s", input);

    if (strcmp(input, "Stop") == 0)
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
            if (strcmp(input, "Stop") == 0)
            {
                break;
            }
            switch (ss_to_base_10(input, radix, &temp))
            {
            case invalid_character:
                printf("The number contains an invalid character\n");
                break;
            case ok_transfer:
                flag = correct;
                if (llabs(max_number) < llabs(temp))
                {
                    max_number = temp;
                }
            }
        }
    }


    switch (flag)
    {
    case correct:
        printf("The maximum modulo of your numbers: %lld\n", max_number);
        transfer_from_int(&input, max_number, 9);
        print_to_radix(input, 9);

        transfer_from_int(&input, max_number, 18);
        print_to_radix(input, 18);

        transfer_from_int(&input, max_number, 27);
        print_to_radix(input, 27);

        transfer_from_int(&input, max_number, 36);
        print_to_radix(input, 36);
        break;
    case not_correct:
        printf("The numbers were not entered or they are incorrect\n");
        break;
    }

    return 0;
}
