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
    long long ttt = 9999999;


    overfprintf(stdout, "%%%lld%% %Ro %d %d %Ro\n", ttt, num1, 100, num1, num1);
    overfprintf(stdout, "Rome : %Ro \nCyckendorf : %Zr\nToBase 16: %Cv  %d\n", num1, num2, num_to, to_base, num_to, to_base);
    overfprintf(stdout, "%d %Cv %d\n", 777, 250, 16, 250);
    overfprintf(stdout, "%to\t%TO\tBase: %d\n", str1, base, str2, base, base);
    int pp1;
    overfprintf(stdout, "abc%n\n", &pp1);
    overfprintf(stdout, "%d\n", pp1); 
    overfprintf(stdout, "Dump int: %mi\nDump uint: %mu\nDump float: %mf\nDump double: %md\nInt: %d\tUINT: %u\tFLOAT: %f\tDOUBLE: %.3f\n", int_value, uint_value, float_value, double_value, int_value, uint_value, float_value, double_value);
    overfprintf(stdout, "%%\n");

    overfprintf(stdout, "%Cv %TO\n", 0, 10, "ABC", 36);
    overfprintf(stdout, "%g\n", -0.00100310);


    char sstr1[100];
    char sstr2[100];
    char * p1 = sstr1;
    char * p2 = sstr2;


    oversprintf(&p1, "%d  Rome: %Ro %d", num1, num1, 1234);
    printf("String %s ass\n", p1); //?????
    oversprintf(&p2, "%mi %p", int_value, &int_value);
    printf("%s", p2);
    overfprintf(stdout, "\nЧисло в 16-й системе: %04X\n", 255);
    return 0;
}



