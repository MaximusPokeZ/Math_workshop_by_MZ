#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 128


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
    char data;
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
    free(node);
}


Node* new_node(char data) 
{
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL) return NULL;
    new->data = data;
    new->children = NULL;
    new->child_count = 0;
    return new;
}

Node* build_tree (char* expression, int* index) 
{
    if (expression[*index] == '\0') return NULL;

    Node* root = new_node(expression[*index]);
    if (root == NULL) return NULL;
    (*index)++;

    while (expression[*index] == ',' || expression[*index] == ' ') (*index)++;

    if (expression[*index] == '(') 
    {
        (*index)++;
        while (expression[*index] != ')') 
        {
            Node* child = build_tree(expression, index);
            Node ** p = (Node**)realloc(root->children, (root->child_count + 1) * sizeof(Node*));
            if (p == NULL) {free_node(root); return NULL;}
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

    fprintf(file, "%*c%c\n", level * 2, ' ', root->data);

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
        Node* root = build_tree(line, &index);
        if (root == NULL) {print_status(invalid_input); continue;}
        to_file(root, outfile, 0);
        fprintf(outfile, "\n");
        free_node(root);
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}
