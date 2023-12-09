#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BUFF_SIZE 1024
#define INITIAL_HASHSIZE  128
#define BUFFER_SIZE 256
#define FOR_HASH 62

typedef enum status_codes 
{
    SUCCESS,
    FILE_OPEN_ERROR,
    MEMORY_ALLOCATION_ERROR,
    INPUT_ERROR
} status_codes;

typedef struct Node 
{
    char* key;
    char* value;
    struct Node* next;
} Node;

typedef struct Hash_table 
{
    unsigned int size;
    Node** table;
} Hash_table;

void print_status_codes(status_codes status) 
{
    switch (status) 
    {
        case FILE_OPEN_ERROR:
            fprintf(stderr, "Error opening the file\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            fprintf(stderr, "Error allocating memory\n");
            break;
        case INPUT_ERROR:
            fprintf(stderr, "Input error\n");
            break;
        default:
            fprintf(stderr, "Unknown error\n");
            break;
    }
}

status_codes diff_file (const char * file1, const char * file2)
{
    int len1 = strlen(file1);
    int len2 = strlen(file2);
    int len = (len1 > len2) ? len1 : len2;

    char * temp1 = (char *)malloc(sizeof(char) * (len * BUFF_SIZE));
    if (temp1 == NULL) return MEMORY_ALLOCATION_ERROR;
    char * temp2 = (char *)malloc(sizeof(char) * (len * BUFF_SIZE));
    if (temp2 == NULL) {free(temp1);return MEMORY_ALLOCATION_ERROR;}

    if (realpath(file1, temp1) == NULL || realpath(file2, temp2) == NULL)
    {
        free(temp1);
        free(temp2);
        return FILE_OPEN_ERROR;
    }

    if (strcmp(temp1, temp2) == 0) 
    {
        free(temp1);
        free(temp2);
        return FILE_OPEN_ERROR;
    }
    free(temp1);
    free(temp2);
    return SUCCESS;
}

void free_table(Hash_table* hashtable) 
{
    for (unsigned int i = 0; i < hashtable->size; i++) 
    {
        Node* current = hashtable->table[i];
        while (current != NULL) 
        {
            Node* next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(hashtable->table);
    free(hashtable);
}

Hash_table* create_hash_table(unsigned int size) 
{
    Hash_table* hashtable = (Hash_table*)malloc(sizeof(Hash_table));
    if (hashtable == NULL) return NULL;

    hashtable->size = size;
    hashtable->table = (Node**)calloc(size, sizeof(Node*));
    if (hashtable->table == NULL) 
    {
        free(hashtable);
        return NULL;
    }
    return hashtable;
}

unsigned int hash(Hash_table* hashtable, char* key) 
{
    unsigned int hashval = 0;

    int base = 62;

    int len = (int)strlen(key);
    for(int i = len-1; i >= 0; i--)
    {
        int value;
        if(key[i] >= 'a' && key[i] <= 'z')
        {
            value = key[i] - 'a' + 10;
        }
        else if(key[i] >= 'A' && key[i] <= 'Z')
        {
            value = key[i] - 'A' + 36;
        }
        else if(key[i] >= '0' && key[i] <= '9')
        {
            value = key[i] - '0';
        }
        if((ULLONG_MAX - hashval) / base < value)
        {
            return 0;
        }
        hashval = hashval * base + value;
    }

    hashval = hashval % hashtable->size;
    return hashval;
}

status_codes insert(Hash_table* hashtable, char* key, char* value) 
{
    size_t len1 = strlen(value);
    size_t len2 = strlen(key);
    if (len1 > 0 && value[len1 - 1] == '\n') { value[len1 - 1] = '\0'; }

    unsigned int index = hash(hashtable, key);
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) return MEMORY_ALLOCATION_ERROR;

    new_node->value = (char*)malloc(sizeof(char) * (len1 + 1));
    if (new_node->value == NULL) 
    {
        free(new_node);
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy(new_node->value, value);
    new_node->key = (char*)malloc(sizeof(char) * (len2 + 1));
    if (new_node->key == NULL) 
    {
        free(new_node->value);
        free(new_node);
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy(new_node->key, key);
    new_node->next = hashtable->table[index];
    hashtable->table[index] = new_node;
    return SUCCESS;
}

status_codes replace_substr(char** str, ssize_t* buffer_size, const char* old, const char* new) 
{
    char* pos = strstr(*str, old);
    while (pos != NULL) 
    {
        size_t len1 = strlen(new);
        size_t len2 = strlen(old);

        if (len1 > len2) 
        {
            size_t new_size = *buffer_size + len1 - len2 + 1;
            char * ptr = NULL;  
            ptr = (char*)realloc(*str, new_size);
            if (ptr == NULL) 
            {
                return MEMORY_ALLOCATION_ERROR;
            }
            *str = ptr;
            *buffer_size = new_size;
            pos = strstr(*str, old);  
        }

        size_t remaining_len = strlen(pos + len2) + 1;
        memmove(pos + len1, pos + len2, remaining_len);       
        memcpy(pos, new, len1);
        pos = strstr(pos + len1, old); 
    }
    return SUCCESS;
}

void resize_hash_table(Hash_table** hashtable, unsigned int new_size) 
{
    Hash_table* new_table = create_hash_table(new_size);

    for (unsigned int i = 0; i < (*hashtable)->size; i++) 
    {
        Node* current = (*hashtable)->table[i];
        while (current != NULL) 
        {
            insert(new_table, current->key, current->value);
            current = current->next;
        }
    }

    free_table(*hashtable);

    *hashtable = new_table;
}

status_codes insert_with_balancing(Hash_table** hashtable, char* key, char* value) 
{
    status_codes result = insert(*hashtable, key, value);

    unsigned int min_length = -1;
    unsigned int max_length = 0;

    for (unsigned int i = 0; i < (*hashtable)->size; i++) 
    {
        Node* current = (*hashtable)->table[i];
        unsigned int chain_length = 0;
        while (current != NULL) 
        {
            chain_length++;
            current = current->next;
        }

        if (chain_length > max_length) 
        {
            max_length = chain_length;
        }

        if (chain_length < min_length) 
        {
            min_length = chain_length;
        }
    }

    if (max_length >= 2 * min_length) 
    {
        if ((*hashtable)->size < max_length) 
        {
            resize_hash_table(hashtable, (*hashtable)->size * 2);
        } 
        else if ((*hashtable)->size > 2 * min_length) 
        {
            resize_hash_table(hashtable, (*hashtable)->size / 2);
        }
    }

    return result;
}

status_codes parsing_file(Hash_table** hashtable, char* input_filename, char* output_filename) 
{
    FILE* output_file = fopen(output_filename, "w");
    if (output_file == NULL) 
    {
        return FILE_OPEN_ERROR;
    }
    if (diff_file(input_filename, output_filename) != SUCCESS)
    {
        return FILE_OPEN_ERROR;
    }
    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL) {return FILE_OPEN_ERROR; fclose(output_file);}



    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, input_file)) != -1) 
    {
        char* ptr = strstr(line, "#define");
        if (ptr != NULL) 
        {
            char def_name[read];
            char value[read];
            if (sscanf(ptr, "#define %255s %255[^\n]", def_name, value) == 2) 
            {
                if (insert_with_balancing(hashtable, def_name, value) != SUCCESS) 
                {
                    fclose(input_file);
                    fclose(output_file);
                    free(line);
                    return MEMORY_ALLOCATION_ERROR;
                }
            } 
            else 
            {
                printf("Error in the format of #define directive: %s", line);
            }
        } 
        else 
        {
            for (unsigned int i = 0; i < (*hashtable)->size; i++) 
            {
                Node* current = (*hashtable)->table[i];
                while (current != NULL) 
                {
                    if (replace_substr(&line, &read, current->key, current->value) != SUCCESS)
                    {
                        free(line);
                        fclose(input_file);
                        fclose(output_file);
                        return MEMORY_ALLOCATION_ERROR;
                    }
                    current = current->next;
                }
            }

        }
        fprintf(output_file, "%s", line);
    }
    free(line);

    fclose(input_file);
    fclose(output_file);


    return SUCCESS;
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return INPUT_ERROR;
    }

    Hash_table* hashtable = create_hash_table(INITIAL_HASHSIZE);
    if (hashtable == NULL) 
    {
        print_status_codes(MEMORY_ALLOCATION_ERROR);
        return MEMORY_ALLOCATION_ERROR;
    }

    status_codes status = parsing_file(&hashtable, argv[1], argv[2]);
    
    
    free_table(hashtable);

    if (status != SUCCESS) 
    {
        print_status_codes(status);
    } 
    else 
    {
        printf("OK!\n");
    }
    
    return status;
}