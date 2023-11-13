#include <stdio.h>
#include <stdlib.h>

enum status_codes
{
    memory_allocation_problem,
    invalid_input,
    success
};

void print_status(enum status_codes status)
{
    switch (status)
    {
        case memory_allocation_problem:
            printf("Memory allocation problem\n");
            break;
        case invalid_input:
            printf("Error: Invalid input\n");
            break;
        default:
            break;
    }
}




int addition(int a, int b) 
{
    // Пока есть перенос, продолжаем складывать
    while (b != 0) 
    {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

enum status_codes check_base(int base, int *mask, int *right)
{
    int power_of_two = 1;
    int pow = 0;
    while (power_of_two < base)
    {
        power_of_two = power_of_two << 1; //умножаем на 2
        pow = addition(pow, 1);
    }
    if (power_of_two == base) //наша база корректна
    {
        *mask = addition(base, -1);
        *right = pow;
        return success;
    }
    return invalid_input;
}


void reverse(char **str, int start, int length)
{
    int i = 0;
    int j = addition(length, -1);
    for (i = start; i < j; i = addition(i, 1), j = addition(j, -1))
    {
        int a = (*str)[i];
        (*str)[i] = (*str)[j];
        (*str)[j] = a;
    }
}

enum status_codes number_to_crnt_base(int number, int base, char **result)
{
    int mask = 0, right = 0;
    int buffer_size = 2, crnt_size = 0, start = 0;
    char *answer = (char *)malloc(sizeof(char) * buffer_size); 
    if (answer == NULL) return memory_allocation_problem;

    enum status_codes status = check_base(base, &mask, &right);
    if (status != success) { free(answer); return status; }

    if (number == 0)
    {
        answer[0] = '0';
        answer[1] = '\0';
        *result = answer;
        return success;
    }

    if (number < 0)
    {
        number = addition(~number, 1); //инвертирование и добавление 1 (~-5 -> 4 -> +1 -> 5)
        answer[crnt_size] = '-';
        crnt_size++;
        start = 1;
    }

    while (number > 0)
    {
        crnt_size = addition(crnt_size, 1);
        if (crnt_size >= buffer_size)
        {
            buffer_size = buffer_size << 1;
            char *temp = (char *) realloc(answer, sizeof(char) * buffer_size);
            if (temp == NULL)
            {
                free(answer);
                return memory_allocation_problem;
            }
            answer = temp;
        }
        int digit = mask & number;
        number >>= right;
        if (digit > 9) answer[addition(crnt_size, - 1)] = (char)(addition(addition(digit, -10), 'A'));
        else answer[addition(crnt_size, - 1)] = (char)(addition(digit, '0'));
    }
   
    reverse(&answer, start, crnt_size);
    answer[crnt_size] = '\0';

    *result = answer;
    return success;
}