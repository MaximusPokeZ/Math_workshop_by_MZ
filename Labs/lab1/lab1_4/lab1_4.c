#include "functions.h"



int main(int argc, char** argv) 
{
    switch (input_checker(argc, argv)) {
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
                    without_arabic(argv[2], "", 'n');
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
                    count_of_latin_or_not_latin(argv[2], "", 'n', 'l');
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
                    count_of_latin_or_not_latin(argv[2], "", 'n', 'n');
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
        case flag_nd:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    switch (file_read_write(argv[3], "w")) {
                        case file_ok:
                            without_arabic(argv[2], argv[3], '-');
                            printf("OK!\n");
                            break;
                        case file_error:
                            printf("Problems with opening the output file!\n");
                            break;

                    }
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
        case flag_ni:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    switch (file_read_write(argv[3], "w")) {
                        case file_ok:
                            count_of_latin_or_not_latin(argv[2], argv[3], '-', 'l');
                            printf("OK!\n");
                            break;
                        case file_error:
                            printf("Problems with opening the output file!\n");
                            break;

                    }
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
        case flag_ns:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    switch (file_read_write(argv[3], "w")) {
                        case file_ok:
                            count_of_latin_or_not_latin(argv[2], argv[3], '-', 'n');                            printf("OK!\n");
                            break;
                        case file_error:
                            printf("Problems with opening the output file!\n");
                            break;

                    }
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
        case flag_na:
            switch (file_read_write(argv[2], "r")) {
                case file_ok:
                    switch (file_read_write(argv[3], "w")) {
                        case file_ok:
                            liters_to_16ss(argv[2], argv[3], '-');
                            printf("OK!\n");
                            break;
                        case file_error:
                            printf("Problems with opening the output file!\n");
                            break;

                    }
                    break;
                case file_error:
                    printf("Problems with opening the input file!\n");
                    break;
            }
            break;
    }

    return 0;
}