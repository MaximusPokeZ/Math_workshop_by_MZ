#include "functions.c"


int main()
{
    int num = 27;
    char *str;
    enum status_codes status = number_to_crnt_base(num, 4, &str);
    if (status == success)
    {
        printf("%s\n", str);
        free(str);
    }
    else print_status(status);


    return 0;
}