#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef struct Indexes
{
    unsigned int* indx;
    unsigned int count_indx;
    char * file_name;
}Indexes;


enum status_codes 
{ 
    status_ok, 
    file_error,
    memory_allocation_problem 
};

void create_shift_table(const char *pattern, int M, int *table) {
    int i = 0;
    int j = -1;
    table[0] = j;

    while (i < M) 
    {
        while (j >= 0 && pattern[i] != pattern[j])
        {
            j = table[j];
        }
        i++;
        j++;
        table[i] = j;
    }
}

enum status_codes find_patterns(Indexes ** results, const char *pattern, ...) 
{
    va_list files;
    va_start(files, pattern);
    const char *crt_file;
    int crnt_file = 0;
    while ((crt_file = va_arg(files, const char *)) != NULL) 
    {
        FILE *file = fopen(crt_file, "r");
        if (file == NULL)
        {
            va_end(files);
            return file_error;
        }
        crnt_file++;
        int M = strlen(pattern);
        int *table = (int *)malloc(sizeof(int) * M);
        if (table == NULL) 
        {
            va_end(files);
            return memory_allocation_problem;
        }

        create_shift_table(pattern, M, table);
        
        char c;
        int pos = 0, j = 0;
        Indexes res;
        res.file_name = (char *)crt_file;
        res.indx = NULL;
        res.count_indx = 0;

        while ((c = fgetc(file)) != EOF)
        {
            while (j >= 0 && c != pattern[j])
            {
                j = table[j];
            }
            pos++;
            j++;
            if (j == M) 
            {
                res.count_indx++;
                unsigned int * indx_ptr = NULL;
                indx_ptr = (unsigned int *)realloc(res.indx, sizeof(unsigned int) * res.count_indx);
                if (indx_ptr == NULL)
                {
                    if (res.indx != NULL) free(res.indx);
                    return memory_allocation_problem;
                }
                res.indx = indx_ptr;
                res.indx[res.count_indx - 1] = pos - j + 1;
                j = table[j];
            } 
        }
        Indexes * res_ptr = NULL;
        res_ptr = (Indexes *)realloc(*results, sizeof(Indexes) * crnt_file);
        if (res_ptr == NULL)
        {
            if (*results != NULL) free (*results);
            return memory_allocation_problem;
        }
        *results = res_ptr;
        (*results)[crnt_file - 1] = res;
        fclose(file);
        free(table);
    }
    va_end(files);
    return status_ok;
}


