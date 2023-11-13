#include "stack_undo.c"

void print_status(enum status_codes status)
{
    switch (status)
    {
        case memory_allocation_problem:
            printf("Memory allocation problem\n");
            break;
        case give_null_pointer:
            printf("Error: Free null ptr\n");
            break;
        case file_open_error:
            printf("Error opening file\n");
            break;
        case invalid_last_name:
            printf("Invalid input for Last Name. Must be a Latin string. 0 < len <= 50\n");
            break;
        case invalid_first_name:
            printf("Invalid input for First Name. Must be a Latin string. 0 < len <= 50\n");
            break;
        case invalid_patronymic:
            printf("Invalid input for Patronymic. Must be a Latin string or an empty string. 0 < len <= 50\n");
            break;
        case invalid_date_of_birth:
            printf("Invalid input for Date of Birth.\n");
            break;
        case invalid_gender:
            printf("Invalid input for Gender. Must be 'M' or 'W'.\n");
            break;
        case invalid_average_income:
            printf("Invalid input for Average Income. Must be a non-negative number.\n");
            break;
        case no_actions_to_undo:
            printf("No actions to undo!\n");
            break;
        default:
            break;
    }
}

bool is_number(const char* str) 
{
    for (int i = 0; str[i] != '\0'; i++) 
    {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

bool is_latin(const char* str) 
{
    for (int i = 0; str[i] != '\0'; i++) 
    {
        if (!isalpha(str[i])) return false;
    }
    return true;
}

int is_valid_gender(char gender) 
{
    return (gender == 'M' || gender == 'W');
}

int is_in_range(int num, int min, int max) 
{
    return (num >= min && num <= max);
}

bool validate_data(Liver liver) 
{
    if (strlen(liver.last_name) > 50 || strlen(liver.first_name) > 50 || strlen(liver.patronymic) > 50 || !is_latin(liver.patronymic) || !is_latin(liver.last_name) || !is_latin(liver.first_name)) return false;
    if (!is_in_range(liver.birth_day, 1, 31) || !is_in_range(liver.birth_month, 1, 12) || !is_in_range(liver.birth_year, 1900, 2023)) return false; 
    if (!is_valid_gender(liver.gender)) return false;
    if (liver.average_income < 0) return false;
    return true; 
}


enum status_codes modify_node_or_add(Node *new_node, Undo_stack * stack, int choice) 
{
    Liver new_l;
    printf("Enter new information:\n");
    char c;
    int len;
    char buffer[BUFF_SIZE];
    enum status_codes status;

    printf("Last Name: ");
    fgets(buffer, sizeof(buffer), stdin);
    len = strlen(buffer) - 1;
    buffer[len] = '\0';
    if (is_latin(buffer) && (len > 0 && len < 51)) strcpy(new_l.last_name, buffer);
    else return invalid_last_name;

    printf("First Name: ");
    fgets(buffer, sizeof(buffer), stdin);
    len = strlen(buffer) - 1;
    buffer[len] = '\0';
    if (is_latin(buffer) && (len > 0 && len < 51)) strcpy(new_l.first_name, buffer);
    else return invalid_first_name;

    printf("Patronymic: ");
    fgets(buffer, sizeof(buffer), stdin);
    len = strlen(buffer) - 1;
    buffer[len] = '\0';
    if ((is_latin(buffer) && (len > 0 && len < 51)) || len == 0) strcpy(new_l.patronymic, buffer);
    else return invalid_patronymic;

    int d, m, y;
    printf("Date of Birth (dd mm yyyy): ");
    if (scanf("%d %d %d", &d, &m, &y) != 3) 
    {
        while ((c = getchar()) != '\n' && c != EOF);
        return invalid_date_of_birth;
    }
    while ((c = getchar()) != '\n' && c != EOF);
    if (is_in_range(d, 1, 31) && is_in_range(m, 1, 12) && is_in_range(y, 1900, 2023))
    {
        new_l.birth_day = d;
        new_l.birth_month = m;
        new_l.birth_year = y;
    }
    else return invalid_date_of_birth;

    printf("Gender (M/W): ");
    fgets(buffer, sizeof(buffer), stdin);
    len = strlen(buffer) - 1;
    buffer[len] = '\0';
    if (len != 1) 
    {
        printf("Invalid input for Gender.\n");
        return invalid_gender;
    }
    if (!is_valid_gender(buffer[0])) return invalid_gender;
    new_l.gender = buffer[0];

    printf("Average Income: ");
    double temp;
    if (scanf("%lf", &temp) != 1) 
    {
        while ((c = getchar()) != '\n' && c != EOF);
        return invalid_average_income;
    }
    while ((c = getchar()) != '\n' && c != EOF);
    if (temp < 0) return invalid_average_income;
    new_l.average_income = temp;

    if (choice == 3) status = push_action(stack, Modify, new_node);
    else {new_node->liver = new_l; status = push_action(stack, Insert, new_node); }
    if (status != success) return status;
    new_node->liver = new_l;
    return success;
}


enum status_codes export_to_file(Node* head, char* filename) 
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) 
    {
        return file_open_error;
    }
    Node* current = head;
    while (current != NULL) 
    {
        fprintf(file, "%s %s %s %d %d %d %c %.5f\n", current->liver.last_name, current->liver.first_name, current->liver.patronymic, current->liver.birth_day, current->liver.birth_month, current->liver.birth_year, current->liver.gender, current->liver.average_income);
        current = current->next;
    }
    fclose(file);
    return success;
}


enum status_codes interact(Node ** head, Undo_stack ** stack, int * modification_count)
{
    int choice, c;
    char buffer[BUFF_SIZE];
    enum status_codes status;

    while (1) 
    {
        printf("\nMenu:\n");
        printf("1. Print residents\n");
        printf("2. Search resident\n");
        printf("3. Modify resident\n");
        printf("4. Remove resident\n");
        printf("5. Add resident\n");
        printf("6. Export data to file\n");
        printf("7. Undo last modification\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice); while ((c = getchar()) != '\n' && c != EOF);
        Liver new_liver;
        if (choice == 1) print_list(*head);
        else if (choice == 2) 
        {
            printf("Enter last name to search: ");
            scanf("%s", buffer); while ((c = getchar()) != '\n' && c != EOF);
            Node* found_node = search_node(*head, buffer);
            if (found_node != NULL) 
            {
                print_node(found_node);
            } 
            else printf("Resident not found.\n");
        } 
        else if (choice == 3) 
        {
            printf("Enter last name to modify: ");
            scanf("%s", buffer); while ((c = getchar()) != '\n' && c != EOF);
            Node* to_modify_node_or_add = search_node(*head, buffer);
            if (to_modify_node_or_add != NULL) 
            {
                status = modify_node_or_add(to_modify_node_or_add, *stack, choice);
                if (status == success)
                {
                    printf("Success modify\n");
                    (*modification_count)++;
                }
                else 
                {
                    if (status == memory_allocation_problem) return status;
                    print_status(status);
                }
            } 
            else printf("Resident not found.\n");
        } 
        else if (choice == 4) 
        {
            Node * p = NULL;
            printf("Enter last name to remove: ");
            scanf("%s", buffer); while ((c = getchar()) != '\n' && c != EOF);
            if (remove_node(head, buffer, &p) == success) 
            {
                printf("Resident with last name '%s' has been removed.\n", buffer);
                if (p != NULL)
                {
                    status = push_action(*stack, Remove, p);
                    if (status != success) return status;
                    else (*modification_count)++;
                }
            }
            else printf("Resident with last name '%s' not found.\n", buffer);
        } 
        else if (choice == 5) 
        {
            Node* new_node = (Node*)malloc(sizeof(Node));
            if (new_node == NULL) return memory_allocation_problem;
            if (modify_node_or_add(new_node, *stack, choice) == success)
            {
                printf("Success insert\n");
                insert_Node(head, new_node);
                (*modification_count)++;
            }
            else 
            {
                printf("Error with insert\n");
                free(new_node);
                if (status == memory_allocation_problem) return status;
            }
        } 
        else if (choice == 6) 
        {
            printf("Enter filename to export data: ");
            scanf("%s", buffer); while ((c = getchar()) != '\n' && c != EOF);
            status = export_to_file(*head, buffer);
            if (status == success) printf("Data exported to '%s'.\n", buffer);
            else print_status(status);
        } 
        else if (choice == 7) 
        {
            status = Undo(*stack, head, *modification_count);
            if (status == success) {printf("Ok undo!\n"); *modification_count = 0;}
            else print_status(status);
        } 
        else if (choice == 8) 
        {
            printf("Exiting...\n");
            return success;
        } 
        else 
        {
            printf("Invalid choice. Try again.\n");
        }
    }
    return not_success;
}

int main() 
{
    Node* head = NULL;
    int len, modification_count = 0;
    char filename[BUFF_SIZE];

    Undo_stack * stack = NULL;
    stack = (Undo_stack*)malloc(sizeof(Undo_stack));
    if (stack == NULL) return memory_allocation_problem;
    enum status_codes status = init_undo_stack(stack, STACK_SIZE);
    if (status != success) {free(stack); return status;}

    printf("Enter the input filename: ");
    fgets(filename, sizeof(filename), stdin);
    len = strlen(filename) - 1;
    filename[len] = '\0';

    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("Error opening file\n");
        return file_open_error;
    }

    char last_name[BUFF_SIZE];
    char first_name[BUFF_SIZE];
    char patronymic[BUFF_SIZE];
    int birth_day;
    int birth_month;
    int birth_year;
    char gender;
    float average_income;

    int number_line = 0; printf("\n");
    while (fscanf(file, "%s %s %s %d %d %d %c %f\n", last_name, first_name, patronymic, &birth_day, &birth_month, &birth_year, &gender, &average_income) != EOF)
    {
        number_line++;
        if (!is_latin(last_name) || !is_latin(first_name) || !(is_latin(patronymic) || strlen(patronymic) == 0) || !is_valid_gender(gender) || average_income < 0) 
        {
            printf("Invalid data. Skipping the line %d\n", number_line);
            continue; 
        }
        
        Liver liver;
        strcpy(liver.last_name, last_name);
        strcpy(liver.first_name, first_name);
        strcpy(liver.patronymic, patronymic);
        liver.birth_day = birth_day;
        liver.birth_month = birth_month;
        liver.birth_year = birth_year;
        liver.gender = gender;
        liver.average_income = average_income;
        
        // Проверяем валидность данных перед добавлением в список
        if (validate_data(liver)) 
        {
            Node * new = create_node(liver);
            if (new == NULL) {free_list(head); return memory_allocation_problem;}
            insert_Node(&head, new);
        } 
        else
        {
            printf("Invalid data. Skipping the line %d\n", number_line);
        }
    }
    fclose(file);


    status = interact(&head, &stack, &modification_count);
    if (status != success)
    {
        print_status(status);
    }
    free_undo_stack(stack);
    free_list(head);

}
