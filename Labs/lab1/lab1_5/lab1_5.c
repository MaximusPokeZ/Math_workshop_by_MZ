#include "functions.h"



int main(int argc, char* argv[])
{
    long double parameters_x_eps[2];

    switch (Input_checker(argc, argv, parameters_x_eps))
    {
    case invalid_input:
        printf("\nInvalid input\n");
        printf("Use %s -help to list all functions\n\n", argv[0]);
        break;
    case info_input:
        print_info();
        break;
    case info_input_help:
        print_help(argv[0]);
        break;
    case invalid_input_flag:
        printf("Invalid flag\n");
        break;
    case undefined_input_flag:
        printf("Undefined flag\n");
        break;
    case overflow_input:
        printf("Overflow_input\n");
        break;
    case underflow_intput:
        printf("Underflow_intput\n");
        break;
    case true_input:
        if (parameters_x_eps[0] > 100 || parameters_x_eps[0] < MIN_EPSILON)
        {
            printf("1e-15 < |X| < 100\n");
            break;
        }
        printf("X = %.3Lf\n", parameters_x_eps[0]);
        printf("Epsilon = %.9Lf\n", parameters_x_eps[1]);
        print_sum_(sum_a(parameters_x_eps[0], parameters_x_eps[1]), 'a');
        print_sum_(sum_b(parameters_x_eps[0], parameters_x_eps[1]), 'b');
        if (parameters_x_eps[0] >= 1)
        {
            printf("For sums 'C' && 'D' x must be in the range |X| < 1\n");
        }
        else
        {
            print_sum_(sum_c(parameters_x_eps[0], parameters_x_eps[1]), 'c');
            print_sum_(sum_d(parameters_x_eps[0], parameters_x_eps[1]), 'd');  
        }
        break;
    }


return 0;


}