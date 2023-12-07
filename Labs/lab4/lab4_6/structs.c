#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef enum status_codes
{
    SUCCESS,
    ERROR_OF_MEMORY,
    WRONG_INPUT,
    ERROR_OF_FILE,
    RUNTIME_ERROR,
    WRONG_NUMBER_OF_BRACKETS,
    WRONG_ARGUMENTS,
    WRONG_OPERATOR
} status_codes;


void print_status(status_codes code) 
{
    switch (code) {
        case SUCCESS:
            printf("Success\n");
            break;
        case ERROR_OF_MEMORY:
            printf("Error: Memory allocation failure\n");
            break;
        case WRONG_INPUT:
            printf("Error: Wrong input\n");
            break;
        case ERROR_OF_FILE:
            printf("Error: File error\n");
            break;
        case RUNTIME_ERROR:
            printf("Error: Runtime error\n");
            break;
        case WRONG_NUMBER_OF_BRACKETS:
            printf("Error: Wrong number of brackets\n");
            break;
        case WRONG_ARGUMENTS:
            printf("Error: Wrong arguments\n");
            break;
        case WRONG_OPERATOR:
            printf("Error: Wrong operator\n");
            break;
        default:
            printf("Unknown status code\n");
            break;
    }
}

typedef struct Stack_node 
{
    char data;
    struct Stack_node *next;
} Stack_node;

typedef struct 
{
    Stack_node *top;
} Stack;

void stack_init(Stack *s) 
{
    s->top = NULL;
}

int is_empty(Stack *s) 
{
    return s->top == NULL;
}

status_codes push(Stack *s, char item) 
{
    Stack_node *new_stack_node = (Stack_node *)malloc(sizeof(Stack_node));
    if (new_stack_node == NULL) 
    {
        return ERROR_OF_MEMORY;
    }

    new_stack_node->data = item;
    new_stack_node->next = s->top;
    s->top = new_stack_node;
    return SUCCESS;
}

char pop(Stack *s) 
{
    if (!is_empty(s)) 
    {
        Stack_node *temp = s->top;
        char data = temp->data;
        s->top = temp->next;
        free(temp);
        return data;
    } 
    return -1;
}


void clear_stack(Stack *s) 
{
    while (!is_empty(s)) 
    {
        pop(s);
    }
}

typedef struct Node
{
    char data;
    struct Node * left;
    struct Node * right;
} Node;

Node* create_node(char data) 
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) return NULL;

    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

void free_node(Node * node)
{
    node->left = node->right = NULL;
    free(node);
    node = NULL;
}

void free_tree(Node * root)
{
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free_node(root);
}


typedef struct Node_to_stack
 {
    Node* data;
    struct Node_to_stack* next;
} Node_to_stack;

typedef struct 
{
    Node_to_stack* top;
} Node_stack;

void node_stack_init(Node_stack* s)
{
    s->top = NULL;
}

int is_node_stack_empty(Node_stack* s) 
{
    return s->top == NULL;
}

status_codes push_node(Node_stack* s, Node* item) 
{
    Node_to_stack* new_stack_node = (Node_to_stack*)malloc(sizeof(Node_to_stack));
    if (new_stack_node == NULL) return ERROR_OF_MEMORY;

    new_stack_node->data = item;
    new_stack_node->next = s->top;
    s->top = new_stack_node;
    return SUCCESS;
}

Node* pop_node(Node_stack* s) 
{
    if (!is_node_stack_empty(s)) 
    {
        Node_to_stack* temp = s->top;
        Node* data = temp->data;
        s->top = temp->next;
        free(temp);
        return data;
    }
    return NULL;
}

void clear_node_stack(Node_stack* s) 
{
    while (!is_node_stack_empty(s)) 
    {
        pop_node(s);
    }
}