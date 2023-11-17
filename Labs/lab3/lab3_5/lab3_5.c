#include "functions.c"



int main(int argc, char* argv[]) 
{
    // if (argc != 3) 
    // {
    //     printf("Usage: <%s> <input_file.txt> <output_file.txt>\n", argv[0]);
    //     return INVALID_DATA;
    // }

    char* infile_str = "input.txt";
    char* outfile_str = "output.txt";
    
    FILE* outfile = fopen(outfile_str, "w");
    if (outfile == NULL) 
    {
        print_status(FILE_ERROR);
        return FILE_ERROR;
    }

    if (diff_file(infile_str, outfile_str) != SUCCESS) 
    {
        fclose(outfile);
        print_status(FILE_ERROR);
        return FILE_ERROR;
    }

    FILE* infile = fopen(infile_str, "r");
    if (infile == NULL) 
    {
        fclose(outfile);
        print_status(FILE_ERROR);
        return FILE_ERROR;
    }



    Student *students = NULL;
    int num_students;
    int capacity;
    status_codes status = read_students_from_file(infile, &students, &num_students, &capacity);
    fclose(infile);
    if (status != SUCCESS)
    {
        if (students != NULL) free_students(students, num_students);
        print_status(status);
        return status;
    }

    user_interface(students, num_students, outfile);

    free_students(students, num_students);
    fclose(outfile);

    return 0;
}