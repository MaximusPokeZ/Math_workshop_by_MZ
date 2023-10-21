#include "functions.c"

int main() 
{
    Indexes *results = NULL;
    enum status_codes status_result = find_patterns(&results, "  ", "in1.txt", "in2.txt", NULL);
    if (status_result == status_ok)
    {
        printf("Ok\n");
        for (int i = 0; results[i].file_name != NULL; i++)
        {
            printf("An occurrence of the substring was found in the file %s:\n", results[i].file_name);
            if (results[i].count_indx == 0) 
            {
                printf("Not found\n");
            }
            else 
            {
                for (int j = 0; j < results[i].count_indx; j++)
                {
                    printf("In line %u, position is %u\n", results[i].str[j], results[i].indx[j]);
                }
            }
            free(results[i].indx);
            free(results[i].str);
        }
        free(results);
    }
    else if (status_result == file_error) 
    {
        printf("File open error\n");
    } 
    else if (status_result == memory_allocation_problem) 
    {
        printf("Memory allocation problem\n");
    }

    return 0;
}
