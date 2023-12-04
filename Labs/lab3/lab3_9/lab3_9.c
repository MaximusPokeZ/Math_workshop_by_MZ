#include "functions.c"

#define SEP_SIZE 10

int main(int argc, char *argv[]) 
{
    if (argc < 3) 
    {
        printf("Usage: %s <file path> <separator1> <separator2> ... <separatorN>\n", argv[0]);
        return 1;
    }

    Tree *tree = init_tree();
    if (tree == NULL) 
    {
        print_status_codes(MEMORY_ALLOCATION_PROBLEM);
        return MEMORY_ALLOCATION_PROBLEM;
    }

    const char *filename = argv[1];

    char *separators = NULL;
    separators = (char *)malloc(SEP_SIZE * sizeof(char));
    if (separators == NULL) 
    {
        print_status_codes(MEMORY_ALLOCATION_PROBLEM);
        return MEMORY_ALLOCATION_PROBLEM;
    }

    for (int i = 2; i < argc; ++i) 
    {
        separators[i - 2] = *(argv[i]);
    }

    Nodes_info Nodes = { .nodes = NULL, .capacity = 0, .size = 0};
    status_codes result;
    result = build_tree_from_file(tree, filename, separators, &Nodes);
    if (result != SUCCESS) 
    {
        print_status_codes(result);
    }
    else
    {
        interactive(tree, &Nodes, separators);
    }
    
    delete_tree(tree->root);
    free(tree);
    free(separators);
    free_nodes_info(&Nodes);
    return result;
}