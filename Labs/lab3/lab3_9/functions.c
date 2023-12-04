#include "node.c"

#define FOR_NODES 10

int is_file_empty(FILE *file) 
{
    int c = fgetc(file);
    if (c == EOF) return 1;
    else 
    {
        ungetc(c, file); 
        return 0;
    }
}

status_codes read_word (FILE *file, char **buffer, int *buffer_size, const char *separators) 
{
    int c;
    int index = 0;

    do {
        c = fgetc(file);
    } while (c != EOF && strchr(separators, c) != NULL);


    while (c != EOF && strchr(separators, c) == NULL) 
    {
        if (index == *buffer_size - 1) 
        {
            *buffer_size *= 2;
            char * ptr = (char *)realloc(*buffer, *buffer_size);
            if (ptr == NULL)
            {
                free(*buffer);
                return MEMORY_ALLOCATION_PROBLEM;
            }
            *buffer = ptr;
        }
        (*buffer)[index++] = (char)c;
        c = fgetc(file);
    }

    (*buffer)[index] = '\0';

    if (index == 0)
    {
        if (c == EOF) return END_OF_FILE;
        else return INVALID_INPUT;
    }
    return SUCCESS;
}

status_codes add_to_nodes(Nodes_info *Nodes, Node *new_node) 
{
    if (Nodes->nodes == NULL) 
    {
        Nodes->nodes = (Node **)malloc(sizeof(Node *) * FOR_NODES);
        if (Nodes->nodes == NULL) return MEMORY_ALLOCATION_PROBLEM;

        Nodes->capacity = FOR_NODES;
        Nodes->size = 0;
    } 
    else 
    {
        if (Nodes->size == Nodes->capacity) 
        {
            Nodes->capacity *= 2;
            Node ** ptr = (Node **)realloc(Nodes->nodes, Nodes->capacity * sizeof(Node *));
            if (ptr == NULL) {free_nodes_info(Nodes); return MEMORY_ALLOCATION_PROBLEM;}
            Nodes->nodes = ptr;
        }
    }
    Nodes->nodes[Nodes->size++] = new_node;
    return SUCCESS;
}


status_codes build_tree_from_file(Tree *tree, const char *filename, const char *separators, Nodes_info * nodes) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL || is_file_empty(file)) return ERROR_WITH_FILE;

    char *buffer = NULL;
    buffer = (char *)malloc(BUFF_SIZE * sizeof(char));
    if (buffer == NULL) 
    {
        fclose(file);
        return MEMORY_ALLOCATION_PROBLEM;
    }
    int buffer_size = BUFF_SIZE;
    status_codes status;
    while (1) 
    {
        status = read_word(file, &buffer, &buffer_size, separators);
        if (status == SUCCESS) 
        {
            Node *new_node = create_node(buffer);
            if (new_node == NULL) 
            {
                free(buffer);
                fclose(file);
                return MEMORY_ALLOCATION_PROBLEM;
            }
            status_new_not status_n_w = NEW;
            tree->root = insert_node(tree->root, new_node, &status_n_w);
            if (status_n_w == NEW)
            {
                if (add_to_nodes(nodes, new_node) == MEMORY_ALLOCATION_PROBLEM)
                {
                    free(buffer);
                    fclose(file);
                    return MEMORY_ALLOCATION_PROBLEM;
                }
            }
        } 
        else if (status == END_OF_FILE) break;
        else 
        {
            free(buffer);
            fclose(file);
            return status;
        }
    }

    free(buffer);
    fclose(file);
    qsort(nodes->nodes, nodes->size, sizeof(Node *), compare_nodes_by_occurrences);
    return SUCCESS;
}

status_codes load_tree_from_file_r(FILE *file, Tree *tree, Nodes_info *nodes)
{
    int occurrences;
    char *buffer = NULL;
    buffer = (char *)malloc(BUFF_SIZE * sizeof(char));
    if (buffer == NULL) return MEMORY_ALLOCATION_PROBLEM;
    int buffer_size = BUFF_SIZE;

    status_codes status = SUCCESS;
    while (1) 
    {
        fscanf(file, "%d", &occurrences);
        status = read_word(file, &buffer, &buffer_size, " ");
        if (status == SUCCESS) 
        {
            Node *new_node = create_node(buffer);
            if (new_node == NULL) 
            {
                free(buffer);
                fclose(file);
                return MEMORY_ALLOCATION_PROBLEM;
            }
            tree->root = insert_node(tree->root, new_node, NULL);
            new_node->count_occurrences = occurrences;

            if (add_to_nodes(nodes, new_node) != SUCCESS) 
            {
                free(buffer);
                fclose(file);
                return MEMORY_ALLOCATION_PROBLEM;
            }
        } 
        else if (status == END_OF_FILE) break;
        else 
        {
            free(buffer);
            fclose(file);
            return status;
        }
    }
    free(buffer);
    return SUCCESS;
}

status_codes load_tree_from_file(Tree *tree, const char *filename, Nodes_info *nodes)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
        return ERROR_WITH_FILE;

    Tree new_tree; 
    new_tree.root = NULL;

    status_codes status = load_tree_from_file_r(file, &new_tree, nodes);
    fclose(file);

    if (status == SUCCESS)
    {
        delete_tree(tree->root);
        tree->root = new_tree.root;
        qsort(nodes->nodes, nodes->size, sizeof(Node *), compare_nodes_by_occurrences);
    }
    else
    {
        delete_tree(new_tree.root);
    }

    return status;
}


void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void interactive(Tree *tree, Nodes_info * Nodes, char * separators)
{
    int option;
    char word[BUFF_SIZE];
    int n;
    status_codes status;
    do
    {
        printf("\nChoose an option:\n");
        printf("1. Search by word\n");
        printf("2. Display the first n most frequently occurring words\n");
        printf("3. Search for the longest and shortest words\n");
        printf("4. Display the depth of the tree\n");
        printf("5. Print tree to FILE\n");
        printf("6. Print tree to console\n");
        printf("7. Build tree from file\n");
        printf("0. Exit\n");

        if (scanf("%d", &option) != 1)
        {
            clear_buffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (option == 1)
        {
            printf("Enter the word to search: ");
            if (scanf("%127s", word) != 1)
            {
                clear_buffer();
                printf("Invalid input. Please enter a word.\n");
                continue;
            }
            Node *found = find_node(tree->root, word);
            if (found)
                printf("<%s> occurs %d time(s) in the file.\n", word, found->count_occurrences);
            else
                printf("<%s> not found in the file.\n", word);
        }
        else if (option == 2)
        {
            printf("Enter the number of words (n): ");
            if (scanf("%d", &n) != 1 || n <= 0)
            {
                clear_buffer();
                printf("Invalid input. Please enter a positive number.\n");
                continue;
            }

            if (find_n_words(n, Nodes) != SUCCESS)
            {
                printf("No words found or invalid input.\n");
            }
        }
        else if (option == 3)
        {
            Node* longest = find_longest_word(tree->root);
            Node* shortest = find_shortest_word(tree->root);

            if (longest != NULL) 
            {
                printf("Longest word: %s\n", longest->data);
            }
            else printf("Null ptr\n");
            if (shortest != NULL) 
            {
                printf("Shortest word: %s\n", shortest->data);
            }
            else printf("Null ptr\n");
        }
        else if (option == 4) 
        {
            int depth = tree_depth(tree->root);
            printf("Depth of the tree: %d\n", depth);
        }
        else if (option == 5) 
        {
            status = save_tree_to_file(tree->root);
            if (status != SUCCESS)
            {
                print_status_codes(status);
            }
            else printf("OK! Save to temp.txt\n");
        }
        else if (option == 6)
        {
            print_tree(tree->root, 0);
        }
        else if (option == 7)
        {
            Nodes_info Nodes_new = { .nodes = NULL, .capacity = 0, .size = 0};
            status = load_tree_from_file(tree, "temp.txt", &Nodes_new);
            if (status != SUCCESS)
            {
                free_nodes_info(&Nodes_new);
                print_status_codes(status);
            }
            else 
            {
                printf("Tree built successfully from file!\n");
                free_nodes_info(Nodes);
                Nodes->nodes = Nodes_new.nodes;
            }
        }
        else if (option == 0)
        {
            printf("Exiting interactive mode.\n");
        }
        else
        {
            clear_buffer();
            printf("Invalid option. Please try again.\n");
        }


    } while (option != 0);
}
