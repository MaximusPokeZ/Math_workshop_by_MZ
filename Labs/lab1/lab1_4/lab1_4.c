#include "functions.h"



int main(int argc, char** argv) 
{
    char * out_name;

    switch (input_checker(argc, argv, &out_name)) {
        case invalid_input:
            printf("Invalid input or flag\n");
            break;
        case info_input:
            print_info();
            break;
        case info_input_help:
            print_help(argv[0]);
            break;
        case flag_d:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    without_arabic(argv[2], out_name);
                    printf("OK!\n");
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
        case flag_i:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    count_of_latin_or_not_latin(argv[2], out_name, 'l');
                    printf("OK!\n");
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
        case flag_s:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    count_of_latin_or_not_latin(argv[2], out_name, 'n');
                    printf("OK!\n");
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
        case flag_a:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    liters_to_16ss(argv[2], "", 'n');
                    printf("OK!\n");
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
    }
    free(out_name);
    return 0;
}