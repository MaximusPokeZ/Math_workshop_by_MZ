#include "functions.h"


int main(int argc, char* argv[])
{

    long double epsilon;
    char * str_epsilon;

    switch(Input_checker(argc, argv, &epsilon, &str_epsilon))
    {
        case good_epsilon:
            printf("Your epsilon: %9Lf\n\n", epsilon);
            printf("The Euler's number:\n");
            print_E(epsilon);
            printf("The PI number:\n");
            print_pi(epsilon);
            printf("The ln(2) number:\n");
            print_ln(epsilon);
            printf("The sqrt(2) number:\n");
            print_sqrt(epsilon);
            printf("The Euler's constant number:\n");
            print_gamma(epsilon);
            break;
        case invalid_input:
            printf("Invalid input\n");
            break;
    } 
 



    return 0;
}