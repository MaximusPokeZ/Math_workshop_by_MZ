#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef struct Indexes
{
    unsigned int* indx;
    unsigned int* str;
    unsigned int count_indx;
    unsigned int count_str;
    char * file_name;
}Indexes;


enum status_codes 
{ 
    status_ok, 
    file_error,
    memory_allocation_problem 
};

void create_shift_table(const char *pattern, int M, int *table, int *count_n) {
    int i = 0;
    int j = -1;
    table[0] = j;

    while (i < M) 
    {
        if (pattern[i] == '\n') (*count_n)++;
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
            fclose(file);
            return memory_allocation_problem;
        }

        int crnt_str = 0;
        create_shift_table(pattern, M, table, &crnt_str);

        char c;
        int pos = 0, j = 0, count_index = 1, count_str = 1;
        int line_number = 1, to_correct_str_numb = crnt_str;
        Indexes res;
        res.file_name = (char *)crt_file;
        res.indx = NULL;
        res.str = NULL;
        res.count_indx = 0;
        res.count_str = 0;

        while ((c = fgetc(file)) != EOF)
        {
            if (c == '\n')
            {
                if (!crnt_str)
                {                
                    pos = 0;
                }
                line_number++;
                crnt_str--;
            }
            while (j >= 0 && c != pattern[j])
            {
                j = table[j];
            }
            pos++;
            j++;
            if (j == M) 
            {
                res.count_indx++;
                if (res.count_indx == count_index)
                {
                    count_index *= 2;
                    unsigned int *indx_ptr = (unsigned int *)realloc(res.indx, sizeof(unsigned int) * count_index);
                    if (indx_ptr == NULL)
                    {
                        if (res.indx != NULL) free(res.indx);
                        return memory_allocation_problem;
                    }
                    res.indx = indx_ptr;
                }
                res.indx[res.count_indx - 1] = pos - j + 1;
                
                res.count_str++;
                if (res.count_str == count_str)
                {
                    count_str *= 2;
                    unsigned int *str_ptr = (unsigned int *)realloc(res.str, sizeof(unsigned int) * count_str);
                    if (str_ptr == NULL)
                    {
                        if (res.str != NULL) { free(res.str); free(res.indx);}
                        return memory_allocation_problem;
                    }
                    res.str = str_ptr;
                }
                res.str[res.count_str - 1] = line_number - to_correct_str_numb;
                
                j = table[j];
            } 
        }
        
        Indexes *res_ptr = (Indexes *)realloc(*results, sizeof(Indexes) * crnt_file);
        if (res_ptr == NULL)
        {
            if (*results != NULL) free(*results);
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


