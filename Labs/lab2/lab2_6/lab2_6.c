#include "for_printf.c"

int main() {
    FILE* file = fopen("input.txt", "r"); // Открываем файл для чтения

    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return 1;
    }

    int intResult;
    unsigned int uintResult;
    int transfer;
    int romanResult;
    char c1 ,c2;
    char  str[20];


    int elementsRead = overfscanf(stdin, "%Ro %s %c %c", &romanResult, str, &c1, &c2);
    printf("%d %s %c %c\n", romanResult,str, c1, c2);
    

    fclose(file); 

    return 0;
}














