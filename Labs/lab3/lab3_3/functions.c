#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STR_BUF 512

typedef struct
{
    int id;
    char name[STR_BUF];
    char surname[STR_BUF];
    double salary;
} Employee;


enum status_codes
{
    success,
    invalid_input,
    memory_allocation_problem,
    error_file_open,
    error_fscanf,
    not_succes
};


void print_status (enum status_codes status)
{
    switch (status)
    {
        case memory_allocation_problem:
            printf("Memory allocation problem\n");
            break;
        case error_file_open:
            printf("Error file open\n");
            break;
        case invalid_input:
            printf("Error with input\n");
            break;
        case error_fscanf:
            printf("Error with fscanf\n");
            break;
        default:
            break;
    }
}

int comp_right(const void *a, const void *b) {
    Employee *first = (Employee *)a, *second =(Employee *)b;
    int res;

    res = first->salary - second->salary;
    if (res != 0) return res;

    res = strcmp(first->surname, second->surname);
    if (res != 0) return res;

    res = strcmp(first->name, second->name);
    if (res != 0) return res;

    return first->id - second->id;
}


int comp_left(const void *a, const void *b)
{
    return comp_right(b, a); 
}


enum status_codes sort_by_increase(Employee **employers, int cnt)
{
    if (cnt < 1) return invalid_input;
    qsort(*employers, cnt, sizeof((*employers)[0]), comp_right);
    return success;
}

enum status_codes sort_by_decrease(Employee **employers, int cnt)
{
    if (cnt < 1) return invalid_input;
    qsort(*employers, cnt, sizeof((*employers)[0]), comp_left);
    return success;
}

void print_employees_to_file(Employee * employees, int cnt, FILE * file)
{
    for (int i = 0; i < cnt; i++)
    {
        fprintf(file, "Salary: %.5lf, Surname: %s,  Name: %s, ID: %d\n",
                employees[i].salary,  employees[i].surname,  employees[i].name,  employees[i].id);
    }
    fprintf(file, "\n");
}

enum status_codes diff_file (const char * file1, const char * file2)
{
    int len1 = strlen(file1);
    int len2 = strlen(file2);
    int len = (len1 > len2) ? len1 : len2;
    char * temp1 = (char *)malloc(sizeof(char) * (len * 100));
    if (temp1 == NULL) return memory_allocation_problem;
    char * temp2 = (char *)malloc(sizeof(char) * (len * 100));
    if (temp2 == NULL) {free(temp1);return memory_allocation_problem;}
    if (realpath(file1, temp1) == NULL || realpath(file2, temp2) == NULL)
    {
        free(temp1);
        free(temp2);
        return not_succes;
    }

    if (strcmp(temp1, temp2) == 0) 
    {
        free(temp1);
        free(temp2);
        return not_succes;
    }
    return success;

}

enum status_codes validate_params(int argc, char *argv[], FILE** file, FILE ** output, char *flag)
{
    if ((argc != 4) || strlen(argv[2]) != 2) return invalid_input;
    if ((argv[2][0] != '/' && argv[2][0] != '-') || (argv[2][1] != 'd' && argv[2][1] != 'a')) return invalid_input;
    *flag = argv[2][1];
    if (diff_file(argv[1], argv[3]) != success) return error_file_open;
    *file = fopen(argv[1], "r");
    if (*file == NULL) return error_file_open;
    *output = fopen(argv[3], "w");
    if (*output == NULL) {fclose(*file); return error_file_open;}
    return success;
}

enum status_codes scan_eployee(FILE *file, Employee *emp)
{
    int id;
    char name[STR_BUF];
    char surname[STR_BUF];
    double salary;

    if(fscanf(file, "%d %s %s %lf\n", &id, name, surname, &salary) != 4)
    {
        return error_fscanf;
    }
    emp->id = id;
    strcpy(emp->name, name);
    emp->salary =  salary;
    strcpy(emp->surname, surname);
    return success;
}