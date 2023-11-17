#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFF_SIZE 256
#define MAX_ID_LENGTH 20
#define MAX_NAME_LENGTH 50


typedef enum 
{
    SUCCESS,
    NOT_SUCCESS,
    FILE_ERROR,
    MEMORY_ERROR,
    INVALID_DATA
} status_codes;

void print_status(status_codes code) 
{
    switch (code) 
    {
        case SUCCESS:
            printf("Operation completed successfully.\n");
            break;
        case FILE_ERROR:
            printf("There was an error opening, reading, or writing to a file.\n");
            break;
        case MEMORY_ERROR:
            printf("There was an error allocating memory.\n");
            break;
        case INVALID_DATA:
            printf("The data provided was invalid.\n");
            break;
        default:
            printf("An unknown error occurred.\n");
            break;
    }
}

status_codes diff_file (const char * file1, const char * file2)
{
    int len1 = strlen(file1);
    int len2 = strlen(file2);
    int len = (len1 > len2) ? len1 : len2;

    char * temp1 = (char *)malloc(sizeof(char) * (len * BUFF_SIZE));
    if (temp1 == NULL) return MEMORY_ERROR;
    char * temp2 = (char *)malloc(sizeof(char) * (len * BUFF_SIZE));
    if (temp2 == NULL) {free(temp1);return MEMORY_ERROR;}

    if (realpath(file1, temp1) == NULL || realpath(file2, temp2) == NULL)
    {
        free(temp1);
        free(temp2);
        return NOT_SUCCESS;
    }

    if (strcmp(temp1, temp2) == 0) 
    {
        free(temp1);
        free(temp2);
        return NOT_SUCCESS;
    }
    return SUCCESS;
}

int is_alpha_string(const char *str) 
{
    while (*str) 
    {
        if (!isalpha(*str)) return 0;
        str++;
    }
    return 1;
}

int is_digits_only(const char *str)
 {
    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;
        str++;
    }
    return 1;  
}



typedef struct Student
{
    unsigned int id;
    char name[50];
    char surname[50];
    char group[20];
    unsigned char *exams;
} Student;

int compare_by_id(const void *a, const void *b)
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return studentA->id - studentB->id;
}

int compare_by_surname(const void *a, const void *b) 
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return strcmp(studentA->surname, studentB->surname);
}

int compare_by_name(const void *a, const void *b) 
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return strcmp(studentA->name, studentB->name);
}

int compare_by_group(const void *a, const void *b) 
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return strcmp(studentA->group, studentB->group);
}

void free_students(Student *students, int num_students) 
{
    for (int i = 0; i < num_students; ++i) { free(students[i].exams);}
    free(students);
}

status_codes search_student(Student *students, int num_students, char choice, Student ** crnt) 
{
    unsigned int search_id;
    char buffer[BUFF_SIZE];
    int len;
    switch (choice) 
    {
        case 'i':
            printf("Enter the student's ID: ");
            fgets(buffer, sizeof(buffer), stdin);
            len = strlen(buffer) - 1;
            buffer[len] = '\0';
            if (len == 0 || len > 6 || is_digits_only(buffer) || sscanf(buffer, "%u", &search_id) != 1)
            {
                printf("Invalid input for ID.\n");
                return INVALID_DATA;
            }
            break;
        case 's':
            printf("Enter the student's surname: ");
            fgets(buffer, sizeof(buffer), stdin);
            len = strlen(buffer) - 1;
            buffer[len] = '\0';
            if (len > MAX_NAME_LENGTH || !is_alpha_string(buffer)) 
            {
                printf("Invalid input for surname.\n");
                return INVALID_DATA;
            }
            break;
        case 'n':
            printf("Enter the student's name: ");
            fgets(buffer, sizeof(buffer), stdin);
            len = strlen(buffer) - 1;
            buffer[len] = '\0';
            if (len> MAX_NAME_LENGTH || !is_alpha_string(buffer)) 
            {
                printf("Invalid input for name.\n");
                return INVALID_DATA;
            }
            break;
        case 'g':
            printf("Enter the student's group: ");
            fgets(buffer, sizeof(buffer), stdin);
            len = strlen(buffer) - 1;
            buffer[len] = '\0';
            if (len > MAX_ID_LENGTH) 
            {
                printf("Invalid input for group.\n");
                return INVALID_DATA;
            }
            break;
        default:
            printf("Invalid search criterion.\n");
            return INVALID_DATA;
    }


    int found = 0;
    for (int i = 0; i < num_students; i++) 
    {
        Student *student = &students[i];
    
        if (choice == 'i' && student->id == search_id) 
        {
            *crnt = student;
            found = 1;
            break;
        }
        else if (choice == 's' && strcmp(student->surname, buffer) == 0) 
        {
            *crnt = student;
            found = 1;
            break;
        }
        else if (choice == 'n' && strcmp(student->name, buffer) == 0) 
        {
            *crnt = student;
            found = 1;
            break;
        }
        else if (choice == 'g' && strcmp(student->group, buffer) == 0) 
        {
            *crnt = student;
            found = 1;
            break;
        }
    }

    if (!found) {printf("No student found based on the given criterion.\n"); return NOT_SUCCESS;}
    return SUCCESS;
}

void print_student_info(FILE * file, Student *student) 
{
    fprintf(file, "\nID: %u\n", student->id);
    fprintf(file, "Surname: %s\n", student->surname);
    fprintf(file, "Name: %s\n", student->name);
    fprintf(file, "Group: %s\n", student->group);
    fprintf(file, "Exam scores: ");
    for (int i = 0; i < 5; i++) { fprintf(file, "%u ", student->exams[i]);}
    fprintf(file, "\n\n");
}

status_codes write_to_file(Student * students, int num_stud, FILE * file)
{
    if (students == NULL || file == NULL) return NOT_SUCCESS;
    for (int i = 0; i < num_stud; i++)
    {
        Student *student = &students[i];
        print_student_info(file, student);
    }
    return SUCCESS;
}

double calculate_average_exam_score(Student *students, int num_students) 
{
    double total_score = 0.0;
    for (int i = 0; i < num_students; i++)
    {
        for (int j = 0; j < 5; j++) 
        {
            total_score += students[i].exams[j];
        }
    }
    return total_score / (5.0 * num_students);
}

void write_high_achievers_to_file(Student *students, int num_students, FILE *file, double average_score) 
{
    fprintf(file, "High achievers (above average):\n");
    for (int i = 0; i < num_students; i++) 
    {
        double total_score = 0.0;
        for (int j = 0; j < 5; j++) { total_score += students[i].exams[j];}
        double average_student_score = total_score / 5.0;

        if (average_student_score > average_score) fprintf(file, "%s %s\n", students[i].surname, students[i].name);
    }
}


status_codes sort_students(Student *students, int num_students, char choice) 
{
    switch (choice) {
        case 'i':
            qsort(students, num_students, sizeof(Student), compare_by_id);
            return SUCCESS;
        case 's':
            qsort(students, num_students, sizeof(Student), compare_by_surname);
            return SUCCESS;
        case 'n':
            qsort(students, num_students, sizeof(Student), compare_by_name);
            return SUCCESS;
        case 'g':
            qsort(students, num_students, sizeof(Student), compare_by_group);
            return SUCCESS;
        default:
            return NOT_SUCCESS;
    }
}



void user_interface(Student *students, int num_students, FILE *file) 
{
    if (students == NULL || file == NULL) return;

    char choice[BUFF_SIZE];
    int len, c;
    double average_score;
    status_codes status;
    Student * crnt = NULL;
    while (1) 
    {
        printf("Choose an action:\n");
        printf("<1> Search for a student\n");
        printf("<2> Sort students\n");
        printf("<3> Write all students to file\n");
        printf("<4> Write to file surnames and names of students with an above-average exam score\n");
        printf("<5> Exit the program\n");
        printf(":- ");
        fgets(choice, sizeof(choice), stdin);
        len = strlen(choice) - 1;
        if (len != 1)
        {
            printf("Invalid choice. Please try again.\n");
            continue;
        }


        switch (*choice) 
        {
            case '1':
                printf("Choose a search criterion (i - by id, s - by surname, n - by name, g - by group): \n:- ");
                fgets(choice, sizeof(choice), stdin);
                len = strlen(choice) - 1;
                if (len != 1)
                {
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }

                status = search_student(students, num_students, *choice, &crnt);
                if (status == SUCCESS) {printf("Person is found\n"); print_student_info(stdout, crnt);}
                else if (status == NOT_SUCCESS) {printf("Person is not found\n");}
                else (print_status(status));
                break;
            case '2':
                printf("Choose a sorting criterion (i - by id, s - by surname, n - by name, g - by group): ");
                fgets(choice, sizeof(choice), stdin);
                len = strlen(choice) - 1;
                if (len != 1)
                {
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
                if (sort_students(students, num_students, *choice) == SUCCESS) printf("OK SORT\n");
                else printf("Invalid choice. Please try again.\n");
                break;
            case '3':
                if (write_to_file(students, num_students, file) == SUCCESS) printf("OK TO FILE\n");
                else print_status(FILE_ERROR);
                break;
            case '4':
                average_score = calculate_average_exam_score(students, num_students);
                write_high_achievers_to_file(students, num_students, file, average_score);
                break;
            case '5':
                printf("Exiting the program.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}


status_codes read_students_from_file(FILE *file, Student **students, int *num_students, int *capacity) 
{
    if (!file) return FILE_ERROR;

    *students = NULL;
    *num_students = 0;
    *capacity = 0;

    char buffer[BUFF_SIZE];
    Student temp;
    int line_num = 0;
    int len;
    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        line_num++;
        if (buffer[0] != '\n') 
        {
            if (*num_students == *capacity) 
            {
                *capacity = (*capacity == 0) ? 1 : (*capacity * 2);
                Student *p = (Student *)realloc(*students, (*capacity) * sizeof(Student));
                if (p == NULL) { return MEMORY_ERROR;}
                *students = p;
            }

            Student *current_student = &temp;

            char *token = strtok(buffer, " ");
            len = strlen(token);
            if (token == NULL || (len == 0 || len > 6) || !is_digits_only (token) ||sscanf(token, "%u", &current_student->id) != 1) 
            {
                printf("Error in line %d\n", line_num); continue;
            }

            token = strtok(NULL, " ");
            if (token == NULL || strlen(token) > 49) 
            {
                printf("Error in line %d\n", line_num); continue;
            }
            strcpy(current_student->name, token);

            token = strtok(NULL, " ");
            if (token == NULL || strlen(token) > 49) 
            {
                printf("Error in line %d\n", line_num); continue;
            }
            strcpy(current_student->surname, token);

            token = strtok(NULL, " ");
            if (token == NULL || strlen(token) > 19) 
            {
                printf("Error in line %d\n", line_num); continue;
            }
            strcpy(current_student->group, token);

            current_student->exams = (unsigned char *)malloc(5 * sizeof(unsigned char));
            if (current_student->exams == NULL) return MEMORY_ERROR;

            int flag = 0;
            for (int i = 0; i < 5; ++i) 
            {
                token = strtok(NULL, " \n");
                if (token == NULL) 
                {
                    free(current_student->exams);
                    flag = 1;
                    break;
                }

                int len = strlen(token);
                unsigned char res = (*token - '0');
                if (len != 1 || res < 2 || res > 5) 
                {
                    free(current_student->exams);
                    flag = 1;
                    break;
                }
                current_student->exams[i] = res;
            }
            if (flag) {printf("Error in line %d\n", line_num); continue;}

            // Проверяем, что в строке нет дополнительных оценок
            token = strtok(NULL, " \n");
            if (token != NULL) 
            {
                free(current_student->exams);
                printf("Error in line %d\n", line_num); continue;
            }

            (*students)[*num_students] = temp;
            (*num_students)++;
        }
    }
    return SUCCESS;
}





