#include "for_printf.c"


int main() {
    int num1 = 123;
    unsigned int num2 = 100;
    int num_to = 100;
    int to_base = 16;
    char * str1 = "abc";
    char * str2 = "ZZ";
    int base = 36;

    int int_value = 12;
    unsigned int uint_value = 144;
    float float_value = 3.14;
    double double_value = 2.71828;

    overfprintf(stdout, "Rome : %Ro \nCyckendorf : %Zr\nToBase 16: %Cv\t%CV\t ToBase 10: %to\t%TO\n", num1, num2, num_to, to_base, num_to, to_base, str1, base, str2, base);
    overfprintf(stdout, "Dump int: %mi\nDump uint: %mu\nDump float: %mf\nDump double: %md\n\n", int_value, uint_value, float_value, double_value);




    char * ptr1;
    char * ptr2;
    ptr1 = (char *)malloc(sizeof(char) * STRING_SIZE);
    if (ptr1 == NULL) return 0;
    ptr2 = (char *)malloc(sizeof(char) * STRING_SIZE);
    if (ptr2 == NULL) return 0;


    oversprintf(&ptr1, "Rome: %Ro\n", num1);
    oversprintf(&ptr2, "%mi\n", int_value);


    printf("Formatted String: %s\n", ptr1);
    printf("Formatted String: %s\n", ptr2);
    free(ptr1);
    free(ptr2);
    return 0;
}



