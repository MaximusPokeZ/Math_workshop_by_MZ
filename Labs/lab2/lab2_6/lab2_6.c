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
    int romanResult1;
    int romanResult2;
    char c1 ,c2;
    char  test_str[20];


    // int num, k;

    // int elementsRead = overfscanf(stdin, NULL, "%d %CV", &num, &k, 11);
    // printf("%d\n", num);
    // printf("%d\n", k);
    
    // float res = 0;
    
    // overfscanf(file, NULL, "%f", &res);
    // printf("\n%.4f\n", res);

    int res1;
    int res2 = 0;
    unsigned int res3 = 0;
    char str[] = "XX 7 001001011";
    oversscanf(NULL, str, "%Ro %d %Zr", &res2, &res1, &res3);
    printf("%d %d %u\n", res2, res1, res3);


    fclose(file); 

    return 0;
}














