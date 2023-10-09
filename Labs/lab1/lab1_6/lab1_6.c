#include "functions.h"


int main(int argc, char* argv[])
{

    long double epsilon;

    switch(Input_checker(argc, argv, &epsilon))
    {
        case good_epsilon:
            printf("Your epsilon: %.9Lf \n", epsilon);
            print_integral_sum(simpson_method(0, 1, epsilon, &func_1), '1');
            print_integral_sum(simpson_method(0, 1, epsilon, &func_2), '2');
            print_integral_sum(simpson_method(0, 1, epsilon, &func_3), '3');
            print_integral_sum(simpson_method(0, 1, epsilon, &func_4), '4');
            break;
        case invalid_input:
            printf("Invalid input\n");
            break;
    } 
 



    return 0;
}