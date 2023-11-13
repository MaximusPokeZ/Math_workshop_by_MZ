#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFF_SIZE 512
#define BUFF_SIZE_LIVER 51
#define STACK_SIZE 10

enum status_codes
{
    memory_allocation_problem,
    give_null_pointer,
    file_open_error,
    invalid_last_name,
    invalid_first_name,
    invalid_patronymic,
    invalid_date_of_birth,
    invalid_gender,
    invalid_average_income,
    no_actions_to_undo,
    
    not_success,
    success
};

typedef struct Liver
{
    char last_name[BUFF_SIZE_LIVER];
    char first_name[BUFF_SIZE_LIVER];
    char patronymic[BUFF_SIZE_LIVER];
    int birth_day;
    int birth_month;
    int birth_year;
    char gender;
    float average_income;
} Liver;


typedef struct Node 
{
    Liver liver;
    struct Node* next;
} Node;




void free_list(Node* head) 
{
    Node* current = head;
    while (current != NULL) 
    {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

Node* create_node(Liver liver) 
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) return NULL;
    new_node->liver = liver;
    new_node->next = NULL;
    return new_node;
}


enum status_codes remove_node(Node** head, const char* last_name, Node** crnt) 
{
    Node* current = (*head);
    Node* prev = NULL;
    while (current != NULL) 
    {
        if (strcmp(current->liver.last_name, last_name) == 0) 
        {
            if (prev == NULL) (*head) = current->next;
            else prev->next = current->next;
            *crnt = current;
            return success;
        }
        prev = current;
        current = current->next;
    }
    return not_success;
}

int compare_nodes(const Node* node1, const Node* node2) 
{
    if (node1->liver.birth_year != node2->liver.birth_year) return node1->liver.birth_year - node2->liver.birth_year;

    if (node1->liver.birth_month != node2->liver.birth_month) return node1->liver.birth_month - node2->liver.birth_month;

    return node1->liver.birth_day - node2->liver.birth_day;
}

void insert_Node(Node** head, Node * new_node) 
{
    // Если список пуст или год рождения нового узла меньше года рождения первого узла в списке, 
    // то новый узел становится новой головой списка
    if (*head == NULL || compare_nodes(*head, new_node) > 0) 
    {
        new_node->next = (*head);
        (*head) = new_node;
    } 
    else
    {
        Node* current = (*head);
        while (current->next != NULL && compare_nodes(current->next, new_node) < 0) { current = current->next; }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void print_node(Node * current)
{
    printf("\nLast Name: %s\n", current->liver.last_name);
    printf("First Name: %s\n", current->liver.first_name);
    printf("Patronymic: %s\n", current->liver.patronymic);
    printf("Date of Birth: %d/%d/%d\n", current->liver.birth_day, current->liver.birth_month, current->liver.birth_year);
    printf("Gender: %c\n", current->liver.gender);
    printf("Average Income: %.5f\n", current->liver.average_income);
}

void print_list(Node* head) 
{
    Node* current = head;
    while (current != NULL) { print_node(current); current = current->next; }
}

Node* search_node(Node* head, char* last_name) 
{
    Node* current = head;
    while (current != NULL) 
    {
        if (strcmp(current->liver.last_name, last_name) == 0) return current;
        current = current->next;
    }
    return NULL;
}

#endif