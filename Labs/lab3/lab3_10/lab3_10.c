#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFF_SIZE 256


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

enum status_codes diff_file (const char * file1, const char * file2)
{
    int len1 = strlen(file1);
    int len2 = strlen(file2);
    int len = (len1 > len2) ? len1 : len2;

    char * temp1 = (char *)malloc(sizeof(char) * (len * BUFF_SIZE));
    if (temp1 == NULL) return memory_allocation_problem;
    char * temp2 = (char *)malloc(sizeof(char) * (len * BUFF_SIZE));
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


typedef struct Node 
{
    char* data;
    struct Node** children;
    int child_count;
} Node;

void free_node(Node *node) 
{
    for (int i = 0; i < node->child_count; i++) 
    {
        free_node(node->children[i]);
    }
    free(node->children);
    free(node->data);
    free(node);
}


Node* new_node(char* data) 
{
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL) return NULL;
    new->data = (char *)malloc(sizeof(char) * strlen(data));
    if (new->data == NULL) {free(new); return NULL;}
    strcpy(new->data, data);
    new->children = NULL;
    new->child_count = 0;
    return new;
}

Node* build_tree (char* expression, int* index) 
{
    if (expression[*index] == '\0') return NULL;

    char buffer[BUFF_SIZE];
    int buffer_index = 0;

    while (expression[*index] != '\0' && expression[*index] != ' ' && expression[*index] != ',' && expression[*index] != '(' && expression[*index] != ')' && expression[*index] != '\n') 
    {
        buffer[buffer_index++] = expression[*index];
        (*index)++;
    }

    buffer[buffer_index] = '\0';

    Node* root = new_node(buffer);
    if (root == NULL) return NULL;

    while (expression[*index] == ',' || expression[*index] == ' ') (*index)++;

    if (expression[*index] == '(') 
    {
        (*index)++;
        while (expression[*index] != ')') 
        {
            Node* child = build_tree(expression, index);
            if (child == NULL) {free_node(root); return NULL;}
            Node ** p = (Node**)realloc(root->children, (root->child_count + 1) * sizeof(Node*));
            if (p == NULL) {free_node(root); free_node(child); return NULL;}
            root->children = p;
            root->children[root->child_count] = child;
            root->child_count++;

            if (expression[*index] == ',' || expression[*index] == ' ') (*index)++;
        }
        (*index)++;
    }
    return root;
}


void to_file (Node* root, FILE* file, int level) 
{
    if (root == NULL) return;

    fprintf(file, "%*c%s\n", level * 2, ' ', root->data);

    for (int i = 0; i < root->child_count; i++) 
    {
        to_file(root->children[i], file, level + 1);
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        printf("Usage: <%s> <input_file.txt> <output_file.txt>\n", argv[0]);
        return invalid_input;
    }

    char* infile_str = argv[1];
    char* outfile_str = argv[2];
    
    FILE* outfile = fopen(outfile_str, "w");
    if (outfile == NULL) 
    {
        print_status(error_file_open);
        return error_file_open;
    }

    if (diff_file(infile_str, outfile_str) != success) 
    {
        fclose(outfile);
        print_status(error_file_open);
        return error_file_open;
    }

    FILE* infile = fopen(infile_str, "r");
    if (infile == NULL) 
    {
        fclose(outfile);
        print_status(error_file_open);
        return error_file_open;
    }

    char line[BUFF_SIZE];
    while (fgets(line, sizeof(line), infile) != NULL) 
    {
        int index = 0;
        while (line[index] == ' ' || line[index] == ',' || line[index] == '(') (index)++;
        Node* root = build_tree(line, &index);
        if (root == NULL) {print_status(invalid_input); continue;}
        to_file(root, outfile, 0);
        free_node(root);
        while (line[index] != '\0' && line[index] != '\n' && line[index] != ')')
        {
            while (line[index] == ' ' || line[index] == ',' || line[index] == '(') (index)++;
            root = build_tree(line, &index);
            to_file(root, outfile, 0);
            if (root != NULL) {free_node(root);}
        }
        fprintf(outfile, "-----------------------------------------------\n");
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}
