#include "functions.c"




int main() 
{

    Numb * mas;
    check_inf_or_not(&mas, 10, 5, 0.100, 0.5, 0.25, 0.1, 0.3);
    if (mas != NULL) {print_arr(mas, 5); free(mas);}
    else         printf("Memmory allocation problem\n");

    check_inf_or_not(&mas, 8, 5, 0.875, 0.125, 0.375, 0.5, 0.123);
    if (mas != NULL) {print_arr(mas, 5); free(mas); }
    else printf("Memmory allocation problem\n");

    check_inf_or_not(&mas, 5, 5, 0.875, 0.125, 0.375, 0.5, 0.125);
    if (mas != NULL) {print_arr(mas, 5); free(mas);}
    else         printf("Memmory allocation problem\n");


    return 0;
}