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
            break;
        case invalid_input:
            return 1;
    } 




    // 
    // 



    return 0;
}