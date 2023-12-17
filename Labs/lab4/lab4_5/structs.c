#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum status_codes
{
    SUCCESS,
    ERROR_WITH_FILE_OPEN,
    INVALID_IN_FILE_INFIX,
    MEMORY_ALLOCATION_ERROR,
    MISSED_LEFT_BRACKET,
    MISSED_RIGHT_BRACKET,
    NEGATIVE_DEGREE,
    INVALID_INPUT,
    DIVISION_BY_ZERO,
    INVALID_OPERATOR,
    INVALID_POSTFIX_EXPRESSION,
    UNCERTAINTLY
} status_codes;

typedef struct stack_node_char
{
    char data;
    struct stack_node_char* next;
} stack_node_char;

typedef struct Stack_for_char
{
    stack_node_char *top;
    int size;
} Stack_for_char;


status_codes create_stack(Stack_for_char **stack)
{
    (*stack) = (Stack_for_char *)malloc(sizeof(Stack_for_char));
    if ((*stack) == NULL) return MEMORY_ALLOCATION_ERROR;
    (*stack)->size = 0;
    (*stack)->top = NULL;
    return SUCCESS;
}

bool is_empty(Stack_for_char *st)
{
    return (st == NULL || st->top == NULL || st->size == 0);
}

int size_stack(Stack_for_char *st)
{
    if (is_empty(st)) return 0;
    return st->size;
}

char top_stack(Stack_for_char *st)
{
    if (is_empty(st)) return '\0';
    return st->top->data;
}

char pop(Stack_for_char *st)
{
    stack_node_char *tmp = st->top;
    char c = tmp->data;
    st->top = st->top->next;
    st->size--;
    free(tmp);
    return c;
}

char peek(Stack_for_char *st) 
{
    if (is_empty(st)) return '\0';
    return st->top->data;
}

status_codes push(Stack_for_char *st, char val)
{
    stack_node_char *tmp = (stack_node_char *)malloc(sizeof(stack_node_char));
    if (tmp == NULL) return MEMORY_ALLOCATION_ERROR;
    tmp->data = val;
    tmp->next = st->top;
    st->top = tmp;
    st->size++;
    return SUCCESS;
}

void free_stack(Stack_for_char *st)
{
    if (st == NULL) return;
    while (!is_empty(st)) { pop(st); }
    free(st);
}

typedef struct stack_node_digit
{
    long long data;
    struct stack_node_digit *next;
} stack_node_digit;

typedef struct Stack_for_digit
{
    stack_node_digit *top;
    int size;
} Stack_for_digit;

status_codes create_stack_d(Stack_for_digit **stack)
{
    (*stack) = (Stack_for_digit *)malloc(sizeof(Stack_for_digit));
    if ((*stack) == NULL) return MEMORY_ALLOCATION_ERROR;
    (*stack)->size = 0;
    (*stack)->top = 0;
    return SUCCESS;
}

bool is_empty_d(Stack_for_digit *st)
{
    return (st == NULL || st->top == NULL || st->size == 0);
}


int size_stack_d(Stack_for_digit *st)
{
    if (is_empty_d(st)) return 0;
    return st->size;
}


long long top_d(Stack_for_digit *st)
{
    if (is_empty_d(st)) return 0;
    return st->top->data;
}

long long pop_d(Stack_for_digit *st)
{
    stack_node_digit *tmp = st->top;
    long long top_symb = tmp->data;
    st->top = st->top->next;
    st->size--;
    free(tmp);
    return top_symb;
}

status_codes push_d(Stack_for_digit *st, long long val)
{
    stack_node_digit *tmp = (stack_node_digit *)malloc(sizeof(stack_node_digit));
    if (tmp == NULL) return MEMORY_ALLOCATION_ERROR;
    tmp->data = val;
    tmp->next = st->top;
    st->top = tmp;
    st->size++;
    return SUCCESS;
}

void free_stack_d(Stack_for_digit *st)
{
    if (st == NULL) return;
    while (!is_empty_d(st)) { pop_d(st); }
    free(st);
}
