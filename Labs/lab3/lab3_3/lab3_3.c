#include "functions.c"

#define employees_SIZE 5

int main(int argc, char *argv[])
{
    FILE *file;
    FILE *output;
    char flag;
    enum status_codes status = validate_params(argc, argv, &file, &output, &flag);
    if (status != success)
    {
        print_status(status);
        return 1;
    }

    int size = employees_SIZE, cnt = 0;

    Employee *employees = (Employee *)malloc(sizeof(Employee) * employees_SIZE);
    if (employees == NULL)
    {
        fclose(file);
        fclose(output);
        print_status(memory_allocation_problem);
        return 0;
    }

    Employee new_emp;
    while ((status = scan_eployee(file, &new_emp)) == success)
    {
        if (size <= cnt)
        {
            size *= 2;
            Employee *new_employees = (Employee *)realloc(employees, sizeof(Employee) * size);
            if (new_employees == NULL)
            {
                free(employees);
                fclose(file);
                fclose(output);
                print_status(memory_allocation_problem);
                return 0;
            }
            employees = new_employees;
        }
        employees[cnt] = new_emp;
        cnt++;
    }
    fclose(file);

    if (flag == 'a')
    {
        sort_by_increase(&employees, cnt); 
    }
    else
    {
        sort_by_decrease(&employees, cnt);
    }

    print_employees_to_file(employees, cnt, output);
    free(employees);
    printf("OK!\n");
    fclose(output);
}