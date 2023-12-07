#include "structs.c"

#define MAX_LEN_FILENAME 10
#define MIN_LEN_FILENAME 5
#define FOR_TXT 5


int operator(char data)
{
    char * operators = "&|~-+<=!?>";
    int i = 0;
    while (*operators != '\0')
    {
        if (*operators == data) return SUCCESS;
        operators++;
    }
    return WRONG_OPERATOR;
}

void print_var(FILE * file, const char * variables, int count)
{
    for (int i = 0; i < count; i++) {fprintf(file, " %c |", variables[i]); }
}

void print_infix(FILE * file, Node * root)
{
    if (root == NULL) return;
    if (operator(root->data) == SUCCESS) fprintf(file, "(");
    print_infix(file, root->left);
    fprintf(file, "%c", root->data);
    if (root->data == '-' || root->data == '+' || root->data == '<')  fprintf(file, ">");
    print_infix(file, root->right);
    if (operator(root->data) == SUCCESS) fprintf(file, ")");
}

void print_header(FILE *file, const char *variables, int count, Node *root) 
{
    print_var(file, variables, count);
    fprintf(file, " ");
    print_infix(file, root);
    fprintf(file, "\n");
}


int priority(char data)
{
    switch(data)
    {
        case '~': return 3;
        case '?': 
        case '!': 
        case '+': 
        case '&': return 2;
        case '|':
        case '-':
        case '<':
        case '=': return 1;
        default: return -1;
    }
}

int is_two_char_operator(char op1, char op2) 
{
    char twoCharOps[][3] = { "->", "<>", "+>"};
    for (int i = 0; i < sizeof(twoCharOps) / sizeof(twoCharOps[0]); i++) {
        if (op1 == twoCharOps[i][0] && op2 == twoCharOps[i][1]) {
            return 1;
        }
    }
    return 0;
}


int repeat_check(char * variables, int count, char symbol)
{
    for (int i = 0; i < count; i++)
    {
        if (variables[i] == symbol) return WRONG_OPERATOR;
    }
    return SUCCESS;
}

int count_var(char ** variables, char * infix)
{
    int length = 0;
    char * temp;
    int len = strlen(infix);
    for (int i = 0; i < len; i++)
    {
        if (isalpha(infix[i]) && repeat_check(*variables, length, infix[i]) == SUCCESS)
        {
            length++;
            temp = (char*)realloc(*variables, length * sizeof(char));
            if (temp == NULL)
            {
                if (*variables != NULL) free(*variables);
                free(infix);
                return ERROR_OF_MEMORY;
            }
            *variables = temp;
            (*variables)[length - 1] = infix[i];
        }
    }
    return length;
}

char * create_filename()
{
    srand(time(NULL));
    int max_len = MAX_LEN_FILENAME;
    int min_len = MIN_LEN_FILENAME;

    int len = rand() % (max_len - min_len + 1) + min_len;

    char * file = (char*)malloc((len + FOR_TXT + 1) * sizeof(char));
    for (int i = 0; i <= len;)
    {
        file[i] = rand() % ('Z' - '0' + 1) + '0';
        if (isalpha(file[i]) || isdigit(file[i])) i++;
    }
    file[len + 1] = '.'; file[len + 2] = 't'; file[len + 3] = 'x'; file[len + 4] = 't';
    file[len + 5] = '\0';
    return file;
}

int solution_from_tree(Node * root, const char * variables, const int * values, const int count)
{
    if (root == NULL) return -1;

    int left = solution_from_tree(root->left, variables, values, count);
    int right = solution_from_tree(root->right, variables, values, count);

    if (isalpha(root->data))
    {
        for (int i = 0; i < count; i++)
        {
            if (root->data == variables[i]) return values[i];
        }
    }

    if (root->data == '0' || root->data == '1') return root->data - '0';

    switch (root->data){
        case '&': return left & right;
        case '|': return left | right;
        case '~': return ~left;
        case '-': return left <= right;
        case '+': return ~(~left | right);
        case '<': return left != right;
        case '=': return left == right;
        case '!': return ~(left & right);
        case '?': return ~(left | right);
        default: return -1;
    }
}

status_codes build_truth_table (Node * root, const int count, const char * variables, char ** output)
{
    char *output_file = create_filename();
    FILE *file = fopen(output_file, "w");
    if (file == NULL)
    {
        free(output_file);
        return ERROR_OF_FILE;
    }
    Node *current = root;

    print_header(file, variables, count, root);

    int num_combinations = 1 << count; // 2^count
    int means[count], result;
    for (int i = 0; i < num_combinations; i++)
    {
        for (int j = 0; j < count; j++)
        {
            int bit = (i >> j) & 1;
            fprintf(file, " %d |", bit);
            means[j] = bit;
        }
        result = solution_from_tree(root, variables, means, count);
        if (result == -1)
        {
            fclose(file);
            free(output_file);
            return WRONG_ARGUMENTS;
        }
        fprintf(file, " %d\n", result);
    }
    fclose(file);
    *output = output_file;
    return SUCCESS;
}

status_codes infix_to_postfix(char **postfix, const char *infix, int * length) 
{
    Stack operator_stack;
    stack_init(&operator_stack);

    int infix_len = strlen(infix);
    char *postfix_result = (char *)malloc((2 * infix_len + 1) * sizeof(char));
    if (postfix_result == NULL) return ERROR_OF_MEMORY;

    int infix_index = 0;
    int postfix_index = 0;

    while (infix_index < infix_len) 
    {
        char symbol = infix[infix_index];

        if (!isspace(symbol)) 
        {
            if (isalnum(symbol)) 
            {
                if (isdigit(symbol) && symbol != '0' && symbol != '1')
                {
                    free(postfix_result); clear_stack(&operator_stack);
                    return WRONG_ARGUMENTS;
                }
                postfix_result[postfix_index++] = symbol;
            } 
            else if (symbol == '(') 
            {
                if (push(&operator_stack, symbol) != SUCCESS) 
                {
                    free(postfix_result); clear_stack(&operator_stack);
                    return ERROR_OF_MEMORY;
                }
            } 
            else if (symbol == ')') 
            {
                while (!is_empty(&operator_stack) && operator_stack.top->data != '(') 
                {
                    postfix_result[postfix_index++] = pop(&operator_stack);
                }
                pop(&operator_stack); // Удаляем '(' из стека
            } 
            else 
            {
                // Обработка двухсимвольных операторов
                if (!is_empty(&operator_stack) && priority(operator_stack.top->data) >= priority(symbol) &&
                    operator_stack.top->data != '(' && is_two_char_operator(operator_stack.top->data, infix[infix_index + 1])) 
                {
                    postfix_result[postfix_index++] = pop(&operator_stack);
                }
                if (push(&operator_stack, symbol) != SUCCESS) 
                {
                    free(postfix_result); clear_stack(&operator_stack);
                    return ERROR_OF_MEMORY;
                }

                if (is_two_char_operator(symbol, infix[infix_index + 1])) 
                {
                    infix_index++; // Пропускаем следующий символ
                }
            }
        }
        infix_index++;
    }

    while (!is_empty(&operator_stack)) 
    {
        postfix_result[postfix_index++] = pop(&operator_stack);
    }

    postfix_result[postfix_index] = '\0';
    *postfix = postfix_result;
    *length = postfix_index;
    clear_stack(&operator_stack);

    return SUCCESS;
}


status_codes build_tree (Node ** tree, char* infix)
{
    char * postfix = NULL;
    int length = 0;
    status_codes result = infix_to_postfix (&postfix, infix, &length);
    if (result != SUCCESS) return result;

    Node_stack tree_stack;
    node_stack_init(&tree_stack);
    int index = 0;
    while (postfix[index] != '\0') 
    {
        char symbol = postfix[index];
        if (isalnum(symbol) || symbol == '0' || symbol == '1') 
        {
            Node* node = create_node(symbol);
            if (node == NULL || push_node(&tree_stack, node) != SUCCESS)
            {
                clear_node_stack(&tree_stack);
                free(postfix);
                return ERROR_OF_MEMORY;
            }
        }
        else 
        {
            Node* new_node = NULL;
            if (is_two_char_operator(symbol, postfix[index + 1])) 
            {
                char two_char_operator[3] = {symbol, postfix[index + 1], '\0'};
                index++;
                new_node = create_node(two_char_operator[0]);
            } 
            else new_node = create_node(symbol);
            
            Node* right_node = pop_node(&tree_stack);
            Node* left_node = pop_node(&tree_stack);
            new_node->left = left_node;
            new_node->right = right_node;
            if (push_node(&tree_stack, new_node) != SUCCESS)
            {
                clear_node_stack(&tree_stack);
                free(postfix);
                return ERROR_OF_MEMORY;
            }
        }
        index++;
    }

    *tree = pop_node(&tree_stack);
    clear_node_stack(&tree_stack);
    free(postfix);

    return SUCCESS;
}

status_codes solution(char* input_file, char** output_file)
{
    FILE * file = fopen(input_file, "r");
    if (file == NULL) return ERROR_OF_FILE;
    
    char * line = NULL;
    size_t buffer_size = 0;
    ssize_t len = getline(&line, &buffer_size, file);

    if (len < 1 || !feof(file))
    {
        free(line);
        fclose(file);
        return ERROR_OF_FILE;
    }
    fclose(file);

    status_codes status;
    Node * root = NULL;
    status = build_tree(&root, line);
    if (status != SUCCESS)
    {
        free(line);
        return status;
    }
    char * variables = NULL;
    int count = count_var(&variables, line);
    free(line);
    if (count < 0)
    {
        free_tree(root); 
        return ERROR_OF_MEMORY;
    }

    status = build_truth_table(root, count, variables, output_file);
    free(variables);
    free_tree(root);
    return status;
}



int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>", argv[0]);
        return WRONG_INPUT;
    }

    char* out = NULL;
    status_codes result = solution(argv[1], &out);

    print_status(result);
    if (result == SUCCESS) printf("Check file %s\n", out);
    if (out != NULL) free(out);
    return result;
}

