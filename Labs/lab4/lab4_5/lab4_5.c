#include "structs.c"


void print_status_codes(status_codes status, char *infix, FILE *out_file, int index) 
{
    switch (status) 
    {
        case SUCCESS:
            fprintf(out_file, "Operation completed successfully.\n");
            break;
        case ERROR_WITH_FILE_OPEN:
            fprintf(out_file, "Error opening the file!\n");
            break;
        case INVALID_IN_FILE_INFIX:
            fprintf(out_file, "Invalid parameter detected!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case MEMORY_ALLOCATION_ERROR:
            fprintf(out_file, "Memory allocation error detected!\n");
            break;
        case MISSED_LEFT_BRACKET:
            fprintf(out_file, "Missing left bracket in the infix expression!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case MISSED_RIGHT_BRACKET:
            fprintf(out_file, "Missing right bracket in the infix expression!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case NEGATIVE_DEGREE:
            fprintf(out_file, "Negative exponent detected!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case INVALID_INPUT:
            fprintf(out_file, "Invalid input detected!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case DIVISION_BY_ZERO:
            fprintf(out_file, "Attempted division by zero in the expression!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case INVALID_OPERATOR:
            fprintf(out_file, "Invalid operator detected in the expression!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case INVALID_POSTFIX_EXPRESSION:
            fprintf(out_file, "Invalid postfix expression detected!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        case UNCERTAINTLY:
            fprintf(out_file, "Uncertain result due to 0^0 expression!\n");
            fprintf(out_file, "Index: %d, Expression: %s\n", index - 1, infix);
            break;
        default:
            fprintf(out_file, "Unknown status code detected!\n");
            break;
    }
}



void print_info(char *expression, char *postfix, long long answer, int len) 
{
    if (expression == NULL) return;

    printf("╔══════════════════════════════════════════\n");
    printf("║ Infix Expression: %-25s\n", expression);
    printf("║ Result: %-33lld\n", answer);
    printf("║ Postfix Expression: ");
    for (int i = 0; i < len; i++) 
    {
        printf("%c", postfix[i]);
    }
    printf("\n");

    printf("╚══════════════════════════════════════════\n");
}

long long bpow(long long base, int exp)
{
    if (exp == 0) return 1;
    if (exp % 2 == 0)
    {
        long long a = bpow(base, exp / 2);
        return a * a;
    }
    return bpow(base, exp - 1) * base;
}

bool is_operator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%');
}

int get_priority(char c) 
{
    switch (c) 
    {
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

status_codes to_postfix_notation(char *infix, char **postfix, ssize_t len, ssize_t * result_len)
{
    if (infix == NULL) return INVALID_IN_FILE_INFIX;
    status_codes status;
    Stack_for_char *stack = NULL;
    status = create_stack(&stack);
    if (status != SUCCESS) return status;

    (*postfix) = (char *)malloc(sizeof(char) * (len * 2));
    if ((*postfix) == NULL)
    {
        free_stack(stack);
        return MEMORY_ALLOCATION_ERROR;
    }

    int index = 0, open_parentheses = 0, close_parentheses = 0;
    char c;
    while (*infix != '\0') 
    {
        c = *infix;
        if (c == ' ' || c == '\t' || c == '\n') infix++;
        else if (isdigit(c) || isalpha(c)) 
        {
            while (isdigit(c) || isalpha(c)) 
            {
                (*postfix)[index++] = c;
                infix++;
                c = *infix;
            }
            (*postfix)[index++] = ' ';
        } 
        else if (is_operator(c)) 
        {
            if (c == '^' && *(infix + 1) == '-')
            {
                free_stack(stack);
                return NEGATIVE_DEGREE;
            }
            while (!is_empty(stack) && get_priority(c) <= get_priority(peek(stack))) {
                (*postfix)[index++] = pop(stack);
                (*postfix)[index++] = ' ';
            }
            push(stack, c);
            infix++;
        } 
        else if (c == '(') 
        {
            push(stack, c);
            infix++;
            open_parentheses++;
        } 
        else if (c == ')') 
        {
            while (!is_empty(stack) && peek(stack) != '(') 
            {
                (*postfix)[index++] = pop(stack);
                (*postfix)[index++] = ' ';
            }
            if (is_empty(stack)) 
            {
                free_stack(stack);
                return MISSED_LEFT_BRACKET;
            }
            pop(stack);
            infix++;
            close_parentheses++;
        } 
        else 
        {
            free_stack(stack);
            return INVALID_IN_FILE_INFIX;
        }
    }

    if (open_parentheses != close_parentheses) 
    {
        free_stack(stack);
        if (open_parentheses > close_parentheses) return MISSED_RIGHT_BRACKET;
        return MISSED_LEFT_BRACKET;
    }

    while (!is_empty(stack)) 
    {
        (*postfix)[index++] = pop(stack);
        (*postfix)[index++] = ' ';
    }

    (*postfix)[index] = '\0'; 
    *result_len = index;
    free_stack(stack);
    return SUCCESS;
}

status_codes solution(char *postfix, long long *res, ssize_t len) 
{
    if (postfix == NULL) return INVALID_IN_FILE_INFIX;
    Stack_for_digit *stack = NULL;
    status_codes status;
    status = create_stack_d(&stack);
    if (status != SUCCESS) 
    {
        free_stack_d(stack);
        return status;
    }

    for (ssize_t i = 0; i < len; ++i) 
    {
        char c = postfix[i];
        if (isdigit(c)) 
        {
            long long num = 0;
            while (isdigit(c)) 
            {
                num = num * 10 + (c - '0');
                i++;
                c = postfix[i];
            }
            push_d(stack, num);
        } 
        else if (c == ' ') continue;
        else if (is_operator(c)) 
        {
            if (is_empty_d(stack)) 
            {
                free_stack_d(stack);
                return INVALID_POSTFIX_EXPRESSION;
            }
            long long operand2 = pop_d(stack);

            if (is_empty_d(stack)) 
            {
                free_stack_d(stack);
                return INVALID_POSTFIX_EXPRESSION;
            }

            long long operand1 = pop_d(stack);

            switch (c) 
            {
                case '+':
                    push_d(stack, operand1 + operand2);
                    break;
                case '-':
                    push_d(stack, operand1 - operand2);
                    break;
                case '*':
                    push_d(stack, operand1 * operand2);
                    break;
                case '/':
                    if (operand2 == 0) 
                    {
                        free_stack_d(stack);
                        return DIVISION_BY_ZERO;
                    }
                    push_d(stack, operand1 / operand2);
                    break;
                case '%':
                    if (operand2 == 0) 
                    {
                        free_stack_d(stack);
                        return DIVISION_BY_ZERO;
                    }
                    push_d(stack, operand1 % operand2);
                    break;
                case '^':
                    if (operand1 == 0 && operand2 == 0)
                    {
                        free_stack_d(stack);
                        return UNCERTAINTLY;
                    }
                    push_d(stack, bpow(operand1, operand2));
                    break;
                default:
                    free_stack_d(stack);
                    return INVALID_OPERATOR;
            }

        } 
        else 
        {
            free_stack_d(stack);
            return INVALID_POSTFIX_EXPRESSION;
        }
    }

    if (stack->size > 1) 
    {
        free_stack_d(stack);
        return INVALID_POSTFIX_EXPRESSION;
    }

    *res = pop_d(stack);
    free_stack_d(stack);

    return SUCCESS;
}

status_codes check_files(char *argv[], int argc)
{
    for (int i = 1; i < argc; i++)
    {
        const char *filename = argv[i];
        FILE *file = fopen(filename, "r");
        if (file == NULL)return ERROR_WITH_FILE_OPEN;

        size_t len = strlen(filename);
        char *errors_file = (char *)malloc(sizeof(char) * (len + 8));
        if (errors_file == NULL)
        {
            fclose(file);
            return MEMORY_ALLOCATION_ERROR;
        }
        snprintf(errors_file, len + 8, "errors_%s", filename);
        FILE *er_file = fopen(errors_file, "w");
        free(errors_file);
        if (file == NULL)
        {
            fclose(file);
            return ERROR_WITH_FILE_OPEN;
        }
        char *line = NULL; size_t size;
        ssize_t count_read;
        int index = 0;
        status_codes status;
        while ((count_read = getline(&line, &size, file)) != -1)
        {
            if (isspace(line[0])) continue;
            char *postfix = NULL;
            ssize_t postix_len;
            status = to_postfix_notation(line, &postfix, count_read, &postix_len);
            if (status != SUCCESS)
            {
                index++;
                print_status_codes(status, line, er_file, index);
                free(postfix);
                continue;
            }
            long long res;
            status = solution(postfix, &res, postix_len);
            if (status != SUCCESS)
            {
                index++;
                print_status_codes(status, line, er_file, index);
                free(postfix);
                continue;
            }
            index++;
            line[count_read - 1] = '\0';
            print_info(line, postfix, res, postix_len);
            free(postfix);
        }
        free(line);
        fclose(file);
        fclose(er_file);
    }
    return SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Enter: %s <file1.txt> ...\n", argv[0]);
        return INVALID_INPUT;
    }
    status_codes status;
    status = check_files(argv, argc);
    return status;
}