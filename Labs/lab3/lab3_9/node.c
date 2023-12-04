#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 128

typedef enum
{
    SUCCESS,
    NOT_SUCCESS,
    INVALID_INPUT,
    END_OF_FILE,
    MEMORY_ALLOCATION_PROBLEM,
    ERROR_WITH_FILE,
    GIVE_NULL_PTR
} status_codes;

typedef enum 
{
    NEW,
    NOT_NEW
} status_new_not;

void print_status_codes(status_codes code) 
{
    switch (code) 
    {
        case SUCCESS:
            printf("Success\n");
            break;
        case NOT_SUCCESS:
            printf("Not success\n");
            break;
        case INVALID_INPUT:
            printf("Invalid input\n");
            break;
        case END_OF_FILE:
            printf("End of file\n");
            break;
        case MEMORY_ALLOCATION_PROBLEM:
            printf("Memory allocation problem\n");
            break;
        case ERROR_WITH_FILE:
            printf("Error with file\n");
            break;
        default:
            printf("Unknown status code\n");
            break;
    }
}

typedef struct Node
{
    struct Node * left;
    struct Node * right;
    char * data;
    int count_occurrences;
} Node;

typedef struct Tree
{
    Node * root;
} Tree;

Tree * init_tree()
{
    Tree * tree = (Tree *)malloc(sizeof(Tree));
    if (tree == NULL) return NULL;
    
    tree->root = NULL;
    return tree;
}

Node * create_node (const char * str)
{
    Node * ptr = (Node *)malloc(sizeof(Node));
    if (ptr == NULL) return NULL;

    ptr->data = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    if (ptr->data == NULL) {free(ptr); return NULL;}
    strcpy(ptr->data, str);
    ptr->left = ptr->right = NULL;
    ptr->count_occurrences = 1;
    return ptr;
}

void delete_node(Node* node) 
{
    if (node == NULL) return;
    free(node->data);  
    free(node);
}

void delete_tree(Node* root) 
{
    if (root != NULL) 
    {
        delete_tree(root->left);  
        delete_tree(root->right);
        delete_node(root);
    }
}

Node * insert_node (Node * root, Node * crnt, status_new_not * status)
{
    if (root == NULL) return crnt;

    int cmp = strcmp(root->data, crnt->data);
    if (cmp == 0)
    {
        root->count_occurrences++;
        delete_node(crnt);
        *status = NOT_NEW;
    }
    else if (cmp > 0) root->left = insert_node(root->left, crnt, status);
    else root->right = insert_node(root->right, crnt, status);

    return root;
}

Node * find_node (Node * root, const char * str)
{
    if (root == NULL) return NULL;

    int cmp = strcmp(root->data, str);
    if (cmp == 0) return root;
    else if (cmp > 0) return find_node(root->left, str);
    else return find_node(root->right, str);

    return root;
}


void print_node (Node * node)
{
    printf("%s (%d)\n", node->data, node->count_occurrences);
}

void print_tree(Node* root, int level) 
{
    if (root != NULL) 
    {
        print_tree(root->right, level + 1);
        for (int i = 0; i < level; ++i) { printf("\t");}
        print_node(root);
        print_tree(root->left, level + 1);
    }
}

typedef struct Nodes_info
{
    Node **nodes;
    size_t size;
    size_t capacity;
} Nodes_info;

int compare_nodes_by_occurrences(const void *a, const void *b)
{
    Node *node_A = *(Node **)a;
    Node *node_B = *(Node **)b;
    return node_B->count_occurrences - node_A->count_occurrences;
}


void free_nodes_info(Nodes_info *Nodes)
{
    if (Nodes->nodes != NULL)
    {
        free(Nodes->nodes);
    }
}

status_codes find_n_words(int n, Nodes_info * Nodes)
{
    if (Nodes == NULL || Nodes->nodes == NULL || n < 1 || n > Nodes->size)
    {
        return NOT_SUCCESS;
    }
    for (int i = 0; i < n; i++)
    {
        print_node(Nodes->nodes[i]);
    }
    return SUCCESS;
}

Node* find_longest_word(Node* root) 
{
    if (root == NULL) return NULL;

    Node* current = root;

    while (current->right != NULL) 
    {
        current = current->right;
    }

    return current;
}

Node* find_shortest_word(Node* root) 
{
    if (root == NULL) return NULL;

    Node* current = root;

    while (current->left != NULL) 
    {
        current = current->left;
    }

    return current;
}

int tree_depth(Node* root) 
{
    if (root == NULL) return 0;

    int left_depth = tree_depth(root->left);
    int right_depth = tree_depth(root->right);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

status_codes save_tree_to_file_recursive(FILE *file, Node *node) 
{
    if (node == NULL) return GIVE_NULL_PTR;

    fprintf(file, "%d%s ", node->count_occurrences, node->data);
    save_tree_to_file_recursive(file, node->left);
    save_tree_to_file_recursive(file, node->right);

    return SUCCESS;
}

status_codes save_tree_to_file(Node *root) 
{
    FILE *file = fopen("temp.txt", "w");
    if (file == NULL) return ERROR_WITH_FILE;

    status_codes result = save_tree_to_file_recursive(file, root);

    fclose(file);
    return result;
}






