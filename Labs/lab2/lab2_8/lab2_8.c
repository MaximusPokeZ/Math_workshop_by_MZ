#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define FOR_START 2

bool remove_zeros(char** num) 
{
    int len = strlen(*num);
    int leading_zeros = 0;
    while (leading_zeros < len && (*num)[leading_zeros] == '0') 
    {
        leading_zeros++;
    }
    
    if (leading_zeros == len)
    {
        (*num)[0] = '0';
        (*num)[1] = '\0';
        return true;
    }
    char * new = (char *)malloc(sizeof(char) * (len - leading_zeros + 1));
    if (new == NULL) return false;
    for (int i = leading_zeros; (*num)[i] != '\0'; i++)
    {
        new[i - leading_zeros] = (*num)[i];
    }
    free(*num);
    *num = new;
    return true;
}

char* sum2nums(char* num1, char* num2, int base) 
{
    int len1 = strlen(num1), len2 = strlen(num2);
    int max_len = (len1 > len2) ? len1 : len2;
    int save = 0;
    char* result = (char*)malloc((max_len + 2) * sizeof(char)); 
    if (result == NULL) return NULL;
    result[0] = '0';
    int i = len1 - 1, j = len2 - 1, k = 1;
    
    for (;(i >= 0 || j >= 0 || save); i--, j--, k++) 
    {
        int digit1 = (i >= 0) ? (isdigit(num1[i]) ? num1[i] - '0' : toupper(num1[i]) + 10 - 'A') : 0;
        int digit2 = (j >= 0) ? (isdigit(num2[j]) ? num2[j] - '0' : toupper(num2[j]) + 10 - 'A') : 0;
        int sum = digit1 + digit2 + save;
        save = sum / base; //сколько добавляем к следующему
        result[max_len + 1 - k] = (sum % base < 10) ? (sum % base) + '0' : (sum % base) + 'A' - 10; //обратно к char
    }
    result[max_len + 1] = '\0';
    return result;
}

char* sum_in_crnt_base(int base, int count, ...) 
{
    va_list args;
    va_start(args, count);
    
    char* result = (char*)malloc(FOR_START * sizeof(char)); 
    if (result == NULL) return NULL;
    result[0] = '0';
    result[1] = '\0';
    if (count == 0) return result;

    for (int i = 0; i < count; i++) 
    {
        char* num = va_arg(args, char*);
        char* temp = sum2nums(result, num, base);
        free(result);
        result = temp;
    }

    va_end(args);
    if (!remove_zeros(&result)) return NULL;
    return result;
}

int main() 
{
    int base = 16; 
    char* num1 = "FF"; 
    char* num2 = "00000000000000000000000000000000000001"; 
    
    char* result = sum_in_crnt_base(base, 2, num1, num2);
    
    printf("Result: %s\n", result);
    

    if (result != NULL) free(result);
    
    return 0;
}
