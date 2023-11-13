#include "node.h"

typedef struct Action 
{
    enum action_type 
    {
        Insert,
        Modify,
        Remove
    } type;
    Node* node;
    Liver prev;
} Action;

typedef struct Undo_stack 
{
    Action* actions;
    int size;
    int capacity;
} Undo_stack;


enum status_codes init_undo_stack(Undo_stack* stack, int capacity) 
{
    stack->actions = (Action*)malloc(sizeof(Action) * capacity);
    if (stack->actions == NULL) return memory_allocation_problem;
    stack->size = 0;
    stack->capacity = capacity;
    return success;
}

enum status_codes free_undo_stack(Undo_stack* stack) 
{
    if (stack->actions == NULL || stack == NULL) return give_null_pointer;
    free(stack->actions);
    stack->size = 0;
    stack->capacity = 0;
    free(stack);
    return success;
}

enum status_codes push_action(Undo_stack* stack, enum action_type type, Node* node) 
{
    if (stack->size == stack->capacity) 
    {
        stack->capacity *= 2;
        Action * p = (Action*)realloc(stack->actions, sizeof(Action) * stack->capacity);
        if (p == NULL) return memory_allocation_problem;
        stack->actions = p;
    }

    stack->actions[stack->size].type = type;
    stack->actions[stack->size].node = node;
    stack->actions[stack->size].prev = node->liver;
    stack->size++;
    return success;
}

enum status_codes Undo(Undo_stack* stack, Node** head, int modification_count) 
{
    printf("%d\n", stack->size);
    printf("%d\n", modification_count);
    Node * temp = NULL;
    if (modification_count < 2) 
    {
        return no_actions_to_undo;
    }
    int undo_count = modification_count / 2;
    for (int i = 0; i < undo_count; i++) 
    {
        Action* action = &stack->actions[stack->size - 1 - i];
        switch (action->type) 
        {
            case Insert:
                remove_node(head, action->node->liver.last_name, &temp);
                if (temp != NULL) free(temp);
                break;
            case Modify:
                action->node->liver = action->prev;
                break;
            case Remove:
                insert_Node(head, action->node);
                break;
        }
    }

    stack->size -= undo_count;
    Action * p = (Action*)realloc(stack->actions, sizeof(Action) * (stack->size + 1));
    if (p == NULL) return memory_allocation_problem;
    stack->actions = p;
    return success;
}
