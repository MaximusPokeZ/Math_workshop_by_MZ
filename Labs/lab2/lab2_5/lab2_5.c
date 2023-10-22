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
    char test[] = "30";
    overfprintf(stdout, "%Ro %d\n", num1, num1);
    overfprintf(stdout, "Rome : %Ro \nCyckendorf : %Zr\nToBase 16: %Cv  %d\n", num1, num2, num_to, to_base, num_to, to_base);
    overfprintf(stdout, "%Cv %d\n", 250, 16, 250);
    overfprintf(stdout, "%to\t%TO\tBase: %d\n", str1, base, str2, base, base);
    // int p1 = 0;
    // int p2 = 0;
    //overfprintf(stdout, "abc%nbbbb%n %d %d", &p1, &p2, p1, p2); ? Not working
    overfprintf(stdout, "Dump int: %mi\nDump uint: %mu\nDump float: %mf\nDump double: %md\nInt: %d\tUINT: %u\tFLOAT: %f\tDOUBLE: %.3f\n", int_value, uint_value, float_value, double_value, int_value, uint_value, float_value, double_value);



    char * ptr1;
    char * ptr2;
    ptr1 = (char *)malloc(sizeof(char) * 100);
    if (ptr1 == NULL) return 0;
    ptr2 = (char *)malloc(sizeof(char) * 100);
    if (ptr2 == NULL) 
    {
        free(ptr1);
        return 0;
    }


    oversprintf(&ptr1, "Rome: %Ro\n", num1);
    oversprintf(&ptr2, "%mi\n %p", int_value, ptr2);


    printf("Formatted String: %s\n", ptr1);
    printf("Formatted String: %s\n", ptr2);
    free(ptr1);
    free(ptr2);
    return 0;
}



