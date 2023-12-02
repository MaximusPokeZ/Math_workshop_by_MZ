#include "struct.h"

#define BUFFER_SIZE 512
#define EPS 1e-9

void clear_buffer(FILE* file) 
{
    int ch;
    while (((ch = fgetc(file)) != '\n') && ch != EOF);
}

void print_in_list(Route_list* list)
{
    route_node* ptr = list->route_head;
    while(ptr != NULL)
    {
        printf("Number of current bus is: %s\n", ptr->bus->bus_id != NULL ? ptr->bus->bus_id : "Unknown");
        printf("X coordinate is: %lf\n", ptr->bus->x);
        printf("Y coordinate is: %lf\n", ptr->bus->y);
        printf("Bus arrival time: %s\n", ptr->bus->arrive != NULL ? ptr->bus->arrive : "Unknown");
        printf("Bus departure time: %s\n", ptr->bus->departure != NULL ? ptr->bus->departure : "Unknown");
        printf("Status of current bus: %s\n\n", ptr->bus->status == START ? "START" : ptr->bus->status == FINAL ? "FINAL" : "INTERMEDIATE");
        ptr = ptr->route_next;
    }
}

void print_all(Buses_list* list)
{
    int count = 0;
    main_node* ptr = list->main_head;
    while(ptr != NULL){
        print_in_list(&(ptr->list_of_routes));
        printf("\n");
        count++;
        ptr = ptr->main_next;
    }
}

int check_coordination_file_in(Route_list* list, double x, double y)
{
    if(list == NULL) return -1;

    int count = 0;
    route_node* ptr = list->route_head;
    while(ptr != NULL)
    {
        if(fabs(ptr->bus->x - x ) < EPS && fabs(ptr->bus->y - y) < EPS) count++;
        ptr = ptr->route_next;
    }
    return count;
}


int check_coordination_file(Buses_list* list, double x, double y)
{
    if(list == NULL) return -1;

    int count = 0;
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        int current = check_coordination_file_in(&(ptr->list_of_routes), x, y);
        if(current == -1) return -1;
        if(current != 0) count++;
        ptr = ptr->main_next;
    }
    return count;
}

double read_and_convert_to_doub(input_status_codes* status, FILE* file) 
{
    char line[BUFFER_SIZE];
    if (!fgets(line, sizeof line, file)) 
    {
        *status = I_INVALID_STRING;
        return 0.0;
    }

    int len = (int)strlen(line);
    if (line[len - 1] != '\n') clear_buffer(file);

    // Проверка наличия символа новой строки в строке
    char* newline_ptr = strchr(line, '\n');
    if (newline_ptr != NULL) *newline_ptr = '\0';  // Заменяем символ новой строки на конец строки

    if (line[0] == '\n' && line[1] == '\0') 
    {
        *status = I_EMPTY_STRING;
        return 0.0;
    }

    char* end;
    errno = 0;
    double val = strtod(line, &end);
    
    if (errno == ERANGE || end != line + len - 1 || (*end != '\0' && *end != '\n')) 
    {
        *status = I_INVALID_STRING;
        return 0.0;
    }

    *status = I_SUCCESS;
    return val;
}

char* read_check_line(input_status_codes* status, FILE* file)
{
    int size = 0, buffer = 128;
    char* result = (char*)malloc(sizeof(char) * (buffer));
    if(result == NULL) { *status = I_MEMORY_ALLOCATION_PROBLEM; return NULL; }

    char c;
    while((c = fgetc(file)) != EOF && c != '\n')
    {
        size++;
        if(size == buffer)
        {
            buffer *= 2;
            char* ptr = (char*) realloc(result, sizeof(char) * (buffer));
            if(ptr == NULL)
            {
                free(result);
                *status = I_MEMORY_ALLOCATION_PROBLEM;
                return NULL;
            }
            else result = ptr;
        }
        result[size - 1] = c;
    }
    result[size] = '\0';

    if(size == 0 && c == EOF)
    {
        *status = I_INVALID_STRING;
        free(result);
        return NULL;
    }
    if(strcmp(result, "\0") == 0 || strcmp(result, "\n") == 0)
    {
        *status = I_EMPTY_STRING;
        free(result);
        return NULL;
    }
    *status = I_SUCCESS;
    return result;
}

status_codes add_in(Route_list* r_list, Bus* bus)
{
    if(r_list == NULL || bus == NULL) return INVALID;

    route_node* new_node = create_route_node(bus);
    if(new_node == NULL) return MEMORY_ALLOCATION_PROBLEM;

    if(r_list->route_head == NULL)
    {
        r_list->route_head = new_node;
        return SUCCESS;
    }
    else if(strcmp(r_list->route_head->bus->arrive, bus->arrive) > 0)
    {
        new_node->route_next = r_list->route_head;
        r_list->route_head = new_node;
        return SUCCESS;
    }
    else 
    {
        route_node* ptr = r_list->route_head;
        while (ptr->route_next != NULL && strcmp(ptr->route_next->bus->arrive, bus->arrive) < 0){ ptr = ptr->route_next; }
        new_node->route_next = ptr->route_next;
        ptr->route_next = new_node;
        return SUCCESS;
    }
}

main_node* find_bus_number(Buses_list* list, char* number_bus)
{
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        if((strcmp(ptr->list_of_routes.route_head->bus->bus_id, number_bus)) == 0) return ptr;
        ptr = ptr->main_next;
    }
    return NULL;
}

status_codes add_out(Buses_list* list, Bus* bus)
{
    status_codes status;
    main_node* new_node = find_bus_number(list, bus->bus_id);
    if(new_node == NULL)
    {
        new_node = create_main_node();
        if(new_node == NULL) return MEMORY_ALLOCATION_PROBLEM;
        status = add_in(&(new_node->list_of_routes), bus);
        if(status != SUCCESS)
        {
            free(new_node);
            return status;
        }
        if(list->main_head == NULL)
        {
            list->main_head = new_node;
            return SUCCESS;
        }
        else
        {
            main_node* ptr = list->main_head;
            while(ptr->main_next != NULL) { ptr = ptr->main_next;}
            ptr->main_next = new_node;
            return SUCCESS;
        }
    }
    else
    {
        status = add_in(&(new_node->list_of_routes), bus);
        return status;
    }
}

input_status_codes parsing_input_data(char* filename, Buses_list* list) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) return I_ERROR_FILE;
    int c = fgetc(file);
    if (c == EOF) { fclose(file); return I_EMPTY_FILE;}
    else fseek(file, 0, SEEK_SET);

    double x, y;
    input_status_codes status = I_SUCCESS;
    x = read_and_convert_to_doub(&status, file);
    if (status != I_SUCCESS) { fclose(file); return status; }
    y = read_and_convert_to_doub(&status, file);
    if (status != I_SUCCESS) { fclose(file); return status;}

    int check_bus_time_arrive = check_coordination_file(list, x, y);
    if(check_bus_time_arrive != 0)
    {
        fclose(file);
        return I_SAME_COORD;
    }
    do
    {
        char* str = read_check_line(&status, file);
        if(status != SUCCESS)
        {
            fclose(file);
            return status;
        }
        char* bus_number = strtok(str, " ");
        char* stop_date = strtok(NULL, " ");
        char* stop_time = strtok(NULL, " ");
        char* go_date = strtok(NULL, " ");
        char* go_time = strtok(NULL, " ");
        char* type = strtok(NULL, " ");

        struct tm stop, go;
        char buffer1[20], buffer2[20];
        strptime(stop_date, "%d.%m.%Y", &stop);
        strptime(stop_time, "%H:%M:%S", &stop);
        strftime(buffer1, 20, "%d.%m.%Y %H:%M:%S", &stop);
        strptime(go_date, "%d.%m.%Y", &go);
        strptime(go_time, "%H:%M:%S", &go);
        strftime(buffer2, 20, "%d.%m.%Y %H:%M:%S", &go);

        status_arriving bs_st = !strcmp(type, "S\n") || !strcmp(type, "S") ? START : !strcmp(type, "I\n") || !strcmp(type, "I") ? INTERMEDIATE : FINAL;

        Bus* cur_bus = (Bus*)malloc(sizeof(Bus));
        if(cur_bus == NULL)
        {
            fclose(file);
            free(str);
            return I_MEMORY_ALLOCATION_PROBLEM;
        }
        cur_bus->status = bs_st;
        cur_bus->x = x;
        cur_bus->y = y;
        cur_bus->arrive = (char*) malloc(sizeof(char) * strlen(buffer1));
        if(cur_bus->arrive == NULL)
        {
            fclose(file);
            free(cur_bus); free(str);
            return I_MEMORY_ALLOCATION_PROBLEM;
        }
        strcpy(cur_bus->arrive, buffer1);
        cur_bus->departure = (char*) malloc(sizeof(char)*(strlen(buffer2) + 1));
        if(cur_bus->departure == NULL)
        {
            fclose(file);
            free(cur_bus->arrive); free(cur_bus); free(str);
            return I_MEMORY_ALLOCATION_PROBLEM;
        }
        strcpy(cur_bus->departure, buffer2);
        cur_bus->bus_id = (char*) malloc(sizeof(char)*(1 + strlen(bus_number)));
        if(cur_bus->bus_id == NULL)
        {
            fclose(file);
            free(cur_bus->arrive); free(cur_bus->departure); free(cur_bus); free(str);
            return I_MEMORY_ALLOCATION_PROBLEM;
        }
        strcpy(cur_bus->bus_id, bus_number);

        status_codes statuss = add_out(list, cur_bus);
        if(statuss == MEMORY_ALLOCATION_PROBLEM)
        {
            fclose(file);
            free(cur_bus->arrive); free(cur_bus->departure); free(cur_bus); free(str);
            return I_MEMORY_ALLOCATION_PROBLEM;
        }
        if(statuss == INVALID)
        {
            fclose(file);
            free(cur_bus->arrive); free(cur_bus->departure); free(cur_bus); free(str);
            return I_INVALID_STRING;
        }
        free(str);
    }
    while(status == SUCCESS);

    fclose(file);
    return I_SUCCESS;
}

void free_list_route(Route_list* list)
{
    route_node* ptr;
    while(list->route_head != NULL)
    {
        ptr = list->route_head;
        list->route_head = list->route_head->route_next;
        free(ptr->bus->bus_id);free(ptr->bus->arrive);
        free(ptr->bus->departure); free(ptr->bus);
        free(ptr);
    }
}

void free_list_buses(Buses_list* list)
{
    main_node* ptr;
    while (list->main_head != NULL)
    {
        ptr = list->main_head;
        list->main_head = list->main_head->main_next;
        free_list_route(&ptr->list_of_routes); free(ptr);
    }
}

check_bus_time_arrive check_coordination_crnt_bus(Route_list* list)
{
    route_node* ptr = list->route_head;
    while(ptr != NULL && ptr->route_next != NULL)
    {
        route_node* ptr2 = ptr->route_next;
        if(fabs(ptr2->bus->x - ptr->bus->x) < EPS && fabs(ptr2->bus->y - ptr->bus->y) < EPS) return CHECK_NOT_SUCCESS;
        ptr = ptr->route_next;
    }
    return CHECK_SUCCESS;
}

check_bus_time_arrive check_coordination(Buses_list* list)
{
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        switch (check_coordination_crnt_bus(&(ptr->list_of_routes))) 
        {
            case CHECK_NOT_SUCCESS: return CHECK_NOT_SUCCESS;
            case CHECK_SUCCESS: break;
        }
        ptr = ptr -> main_next;
    }
    return CHECK_SUCCESS;
}



same_time check_time_crnt_bus(Route_list* list)
{
    route_node* ptr = list->route_head;
    while(ptr != NULL && ptr->route_next != NULL)
    {
        route_node* ptr2 = ptr->route_next;
        int time_arrive = strcmp(ptr->bus->arrive, ptr2->bus->arrive);
        int time_departure = strcmp(ptr->bus->departure, ptr2->bus->departure);
        int time_all = strcmp(ptr->bus->arrive, ptr->bus->departure);

        if(time_arrive == 0 && time_departure == 0) return SAME_TIME_ALL;
        if(time_arrive == 0) return SAME_TIME_ARRIVE;
        if(time_departure == 0) return SAME_TIME_DEPARTURE;
        if(time_all == 0 || strcmp(ptr2->bus->arrive, ptr2->bus->departure) == 0) return SAME_TIME_ARRIVING_AND_DEPARTURE;
        if(time_all > 0 || strcmp(ptr2->bus->arrive, ptr2->bus->departure) > 0) return DEPARTURE_LESS_ARRIVE;
        ptr = ptr->route_next;
    }

    route_node* ptrr = list->route_head;
    while(ptrr != NULL)
    {
        route_node* ptr_2 = ptrr->route_next;
        while(ptr_2 != NULL)
        {
            if(strcmp(ptrr->bus->departure, ptr_2->bus->departure) == 0) return SAME_TIME_DEPARTURE;
            ptr_2 = ptr_2->route_next;
        }
        ptrr = ptrr->route_next;
    }

    ptr = list->route_head;
    while(ptr != NULL)
    {
        ptrr = ptr->route_next;
        while(ptrr != NULL)
        {
            if(strcmp(ptrr->bus->arrive, ptr->bus->departure) <= 0) return DEPARTURE_MORE_THAN_ARRIVING;
            ptrr = ptrr->route_next;
        }
        ptr = ptr->route_next;
    }
    return SUCCESS_TIME;
}

same_time check_time_bus(Buses_list* list)
{
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        same_time status = check_time_crnt_bus(&(ptr->list_of_routes));
        switch (status) 
        {
            case SUCCESS_TIME: break;
            default: return status;
        }
        ptr = ptr->main_next;
    }
    return SUCCESS_TIME;
}


check_bus_time_arrive check_in_list(Route_list* list)
{
    int start, finish = 0;
    if(list->route_head->bus->status != START) return CHECK_NOT_SUCCESS;
    else start = 1;

    route_node* ptr = list->route_head;
    route_node* pre_final = NULL;
    while(ptr->route_next != NULL)
    {
        pre_final = ptr;
        ptr = ptr->route_next;
        if( ptr->bus->status == FINAL && start)
        {
            start = 0;
            finish = 1;
            continue;
        }
        else if(ptr->bus->status == START && finish){
            finish = 0;
            start = 1;
            continue;
        }
        else if(ptr->route_next == NULL) break;
        else if(ptr->bus->status == INTERMEDIATE && start && !finish) continue;
        else return CHECK_NOT_SUCCESS;
    }

    if(ptr->bus->status != FINAL)return CHECK_NOT_SUCCESS;
    if(pre_final->bus->status == FINAL && ptr->bus->status == FINAL) return CHECK_NOT_SUCCESS;
    return CHECK_SUCCESS;
}

check_bus_time_arrive check_all(Buses_list* list)
{
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        switch (check_in_list(&(ptr->list_of_routes))) {
            case CHECK_NOT_SUCCESS: return CHECK_NOT_SUCCESS;
            case CHECK_SUCCESS: break;
        }
        ptr = ptr->main_next;
    }
    return CHECK_SUCCESS;
}

int count_route_in(Route_list* list)
{
    if(list == NULL) return -1;

    int count = 0;
    route_node* ptr = list->route_head;
    while(ptr != NULL){
        if(ptr->bus->status == START || ptr->bus->status == FINAL) count++;
        ptr = ptr -> route_next;
    }
    if(count % 2 != 0)return -1;
    count /= 2;
    return count;
}

char* bus_id_route(Buses_list* list, status_codes* status, max_min type)
{
    if(list == NULL)
    {
        *status = INVALID; return NULL;
    }
    int max = -1;
    int min = INT_MAX;
    main_node* max_route = NULL;
    main_node* min_route = NULL;
    main_node* ptr = list->main_head;
    while(ptr != NULL){
        int current = count_route_in(&(ptr->list_of_routes));
        if(current == -1)
        {
            *status = INVALID;
            return NULL;
        }
        if(current >= max)
        {
            max = current;
            max_route = ptr;
        }
        if(current <= min)
        {
            min = current;
            min_route = ptr;
        }
        ptr = ptr -> main_next;
    }

    if(max_route == NULL || min_route == NULL)
    {
        *status = INVALID;
        return NULL;
    }
    else
    {
        if(type == MAX) return max_route->list_of_routes.route_head->bus->bus_id;
        if(type == MIN) return min_route->list_of_routes.route_head->bus->bus_id;
    }
    return NULL;
}

double length_by_x_y(route_node* first, route_node* second)
{
    if(first == NULL || second == NULL)return -1;
    return sqrt((first->bus->x - second->bus->x) * (first->bus->x - second->bus->x) + (first->bus->y - second->bus->y) * (first->bus->y - second->bus->y));
}

double track_length_in(Route_list* list)
{
    if(list == NULL)return -1.0;

    double length = 0l;
    route_node* ptr = list->route_head;
    while(ptr->route_next != NULL)
    {
        route_node* ptr2 = ptr->route_next;
        double current = length_by_x_y(ptr, ptr2);
        if(fabs(current + 1) < EPS) return -1l;
        length += current;
        ptr = ptr->route_next;
    }
    return length;
}

char* bus_id_length_track(Buses_list* list, status_codes* status, max_min type, double* len)
{
    if(list == NULL) { *status = INVALID; return NULL;}

    double max = -1l;
    double min = MAXFLOAT;
    main_node* max_route = NULL;
    main_node* min_route = NULL;
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        double current = track_length_in(&(ptr->list_of_routes));
        if(fabs(current + 1) < EPS) { *status = INVALID; return NULL;}
        if(current > max)
        {
            max = current;
            max_route = ptr;
        }
        if(current < min)
        {
            min = current;
            min_route = ptr;
        }
        ptr = ptr -> main_next;
    }
    if(max_route == NULL || min_route == NULL) { *status = INVALID; return NULL;}
    else
    {
        if(type == MAX)
        {
            *len = max;
            return max_route->list_of_routes.route_head->bus->bus_id;
        }
        else if(type == MIN)
        {
            *len = min;
            return min_route->list_of_routes.route_head->bus->bus_id;
        }
    }
    return NULL;
}

double longest_route(Route_list* list, max_min type)
{
    if(list == NULL) return -1;

    double max = -1l, min = MAXFLOAT, size_route = 0l;
    route_node* ptr = list->route_head;
    while(ptr != NULL && ptr->route_next != NULL)
    {
        route_node* ptr2 = ptr->route_next;
        double current = length_by_x_y(ptr, ptr2);
        if(current == -1) return -1;
        size_route += current;
        if(ptr2->bus->status == FINAL)
        {
            if(size_route > max) max = size_route;
            if(size_route < min) min = size_route;
            size_route = 0l;
            ptr = ptr2->route_next;
            continue;
        }
        ptr = ptr->route_next;
    }
    if(type == MAX) return max;
    if(type == MIN) return min;
    return 0.0;
}

char* max_route_length_bus_id (Buses_list* list, status_codes* status, max_min type, double* len){
    if(list == NULL) { *status = INVALID; return NULL;}

    double max = -1l, min = MAXFLOAT;
    main_node* max_route = NULL;
    main_node* min_route = NULL;
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        double current = longest_route(&(ptr->list_of_routes), type);
        if(fabs(current + 1) < EPS)
        {
            *status = INVALID;
            return NULL;
        }
        if(current > max)
        {
            max = current;
            max_route = ptr;
        }
        if(current < min)
        {
            min = current;
            min_route = ptr;
        }
        ptr = ptr -> main_next;
    }
    if(max_route == NULL || min_route == NULL){ *status = INVALID; return NULL;}
    else
    {
        if(type == MAX)
        {
            *len = max;
            return max_route->list_of_routes.route_head->bus->bus_id;
        }
        if(type == MIN)
        {
            *len = min;
            return min_route->list_of_routes.route_head->bus->bus_id;
        }
    }
    return NULL;
}

double long_short_stop_in(Route_list* list, max_min type)
{
    if(list == NULL) return -1;

    double max = -1l;
    double min = MAXFLOAT;
    route_node* ptr = list->route_head;
    while(ptr != NULL)
    {
        struct tm in, out;
        strptime(ptr->bus->arrive, "%d.%m.%Y %H:%M:%S", &in);
        strptime(ptr->bus->departure, "%d.%m.%Y %H:%M:%S", &out);
        tzset();
        in.tm_isdst = -1;
        out.tm_isdst = -1;
        time_t first = mktime(&in);
        time_t second = mktime(&out);

        double difference = difftime(second, first);

        if(difference > max ) max = difference;
        if(difference < min) min = difference;
        ptr = ptr->route_next;
    }
    if(type == MAX) return max;
    if(type == MIN) return min;
    return 0.0;
}

char* long_short_stop (Buses_list* list, status_codes* status, max_min type, double* len)
{
    if(list == NULL) { *status = INVALID; return NULL; }

    double max = -1l, min = MAXFLOAT;
    main_node* max_route = NULL;
    main_node* min_route = NULL;
    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        double current = long_short_stop_in(&(ptr->list_of_routes), type);
        if(fabs(current + 1) < EPS) { *status = INVALID; return NULL; }

        if(current > max)
        {
            max = current;
            max_route = ptr;
        }
        if(current < min)
        {
            min = current;
            min_route = ptr;
        }
        ptr = ptr -> main_next;
    }
    if(max_route == NULL || min_route == NULL) { *status = INVALID; return NULL;}
    else
    {
        if(type == MAX)
        {
            *len = max;
            return max_route->list_of_routes.route_head->bus->bus_id;
        }
        if(type == MIN)
        {
            *len = min;
            return min_route->list_of_routes.route_head->bus->bus_id;
        }
    }
    return NULL;
}

double stop_time_in(Route_list* list)
{
    if(list == NULL) return -1;
    double sum = 0l;

    route_node* ptr = list->route_head;
    while(ptr != NULL)
    {
        struct tm in, out;
        strptime(ptr->bus->arrive, "%d.%m.%Y %H:%M:%S", &in);
        strptime(ptr->bus->departure, "%d.%m.%Y %H:%M:%S", &out);

        tzset();
        in.tm_isdst = -1;
        out.tm_isdst = -1;
        time_t first = mktime(&in);
        time_t second = mktime(&out);

        double difference = difftime(second, first);
        sum += difference;
        ptr = ptr->route_next;
    }
    return sum;
}

char* stop_time(Buses_list* list, status_codes* st, double* len)
{
    if(list == NULL){ *st = INVALID; return NULL;}
    double max = -1l;

    main_node* max_route = NULL;

    main_node* ptr = list->main_head;
    while(ptr != NULL)
    {
        double current = stop_time_in(&(ptr->list_of_routes));
        if(fabs(current + 1) < EPS) { *st = INVALID; return NULL; }
        if(current > max)
        {
            max = current;
            max_route = ptr;
        }
        ptr = ptr -> main_next;
    }
    if(max_route == NULL)
    {
        *st = INVALID;
        return NULL;
    }
    else
    {
        *len = max;
        return max_route->list_of_routes.route_head->bus->bus_id;
    }
}

void interactive(Buses_list* list)
{
    while(1) 
    {
        printf("\nChoose action:\n");
        printf("a - the bus with the number of routes\n");
        printf("c - the bus with the track length\n");
        printf("e - the bus with the route length\n");
        printf("g - the bus with the stop length\n");
        printf("w - the bus with the downtime\n");
        printf("i - info about all bus by sorted\n");
        printf("q - exit\n");
        printf("?-: ");
        char act;
        fflush(stdin);
        if(scanf("%c", &act) == EOF) break;
        char c;
        if ((c = getchar()) != '\n' && c != EOF) 
        {
            clear_buffer(stdin);
            printf("INPUT ERROR\n");
            continue;
        }
        status_codes status = SUCCESS;
        max_min comprasion_type;

        if (act == 'a' || act == 'c' || act == 'e' || act == 'g' || act == 'w') 
        {
            printf("Choose comparison type:\n");
            printf("1 - Greater\n");
            printf("2 - Smaller\n");
            printf("?-: ");
            int compType;
            if(scanf("%d", &compType) == EOF) break;
            if ((c = getchar()) != '\n' && c != EOF) 
            {
                clear_buffer(stdin);
                printf("INPUT ERROR\n");
                continue;
            }
            if (compType == 1) comprasion_type = MAX;
            else if (compType == 2) comprasion_type = MIN;
            else 
            {
                printf("Invalid comparison type\n");
                continue;
            }
        }

        if (act == 'a') 
        {
            char* bus_name = bus_id_route(list, &status, comprasion_type);
            if(status == INVALID)
            {
                printf("ERROR with a!\n"); break;
            }
            printf("The bus with the number of routes is: %s\n", bus_name);
        }
        else if (act == 'c') 
        {
            double length;
            char* bus_name = bus_id_length_track(list, &status, comprasion_type, &length);
            if(status == INVALID)
            {
                printf("ERROR with c!\n"); break;
            }
            printf("The bus with the track length (%lf) is: %s\n", length, bus_name);
            continue;
        }
        else if (act == 'e') 
        {
            double routeLength;
            char* bus_name = max_route_length_bus_id(list, &status, comprasion_type, &routeLength);
            if(status == INVALID) 
            {
                printf("ERROR with e!\n"); break;
            }
            printf("The bus with the route length (%lf) is: %s\n", routeLength, bus_name);
        }
        else if (act == 'g') 
        {
            double stopLength;
            char* bus_name = long_short_stop(list, &status, comprasion_type, &stopLength);
            if(status == INVALID)
            {
                printf("ERROR with g!\n"); break;
            }
            printf("The bus with the stop length (%lf): %s\n", stopLength, bus_name);
        }
        else if (act == 'w') 
        {
            double downtime;
            char* bus_name = stop_time(list, &status, &downtime);
            if(status == INVALID)
            {
                printf("ERROR with w!\n"); break;
            }
            printf("The bus with the downtime (%lf): %s\n", downtime, bus_name);
        }
        else if (act == 'i') 
        {
            printf("All transports :\n");
            print_all(list);
        }
        else if (act == 'q') 
        {
            printf("Goodbye!\n");
            break;
        }
    }
}
