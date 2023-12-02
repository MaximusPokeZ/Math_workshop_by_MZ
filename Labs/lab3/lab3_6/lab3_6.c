#include "functions.c"


int main(int argc, char** argv) 
{
    Buses_list main_list;

    if(argc < 2)
    {
        printf("too few arguments\n");
        return 1;
    }
    main_list.main_head = NULL;

    for(int i = 1; i < argc; ++i)
    {
        switch(parsing_input_data(argv[i], &main_list))
        {
            case I_MEMORY_ALLOCATION_PROBLEM:
                printf("MEMORY ALLOCATION PROBLEM\n");
                free_list_buses(&main_list);
                return I_MEMORY_ALLOCATION_PROBLEM;
            case I_ERROR_FILE:
                printf("Problem with open file: %s\n", argv[i]);
                free_list_buses(&main_list);
                return I_ERROR_FILE;
            case I_INVALID_STRING:
                break;
            case I_EMPTY_FILE:
                printf("Enter empty file: %s\n", argv[i]);
                free_list_buses(&main_list);
                return I_EMPTY_FILE;
            case I_EMPTY_STRING:
                printf("There is empty string in file!\n");
                free_list_buses(&main_list);
                return I_EMPTY_STRING;
            case I_SAME_COORD:
                printf("Same coordination in different files\n");
                free_list_buses(&main_list);
                return I_SAME_COORD;
            case I_SUCCESS:
                break;
        }
    }
    if(check_all(&main_list) == CHECK_NOT_SUCCESS)
    {
        printf("Invalid input\n");
        free_list_buses(&main_list);
        return 0;
    }
    switch (check_time_bus(&main_list)) 
    {
        case SAME_TIME_ARRIVING_AND_DEPARTURE:
            printf("Error: You have entered the same arrival and departure time for one bus.\n");
            free_list_buses(&main_list);
            return SAME_TIME_ARRIVING_AND_DEPARTURE;
        case SAME_TIME_ALL:
            printf("Error: You have entered the same time of arrival and departure for one bus.\n");
            free_list_buses(&main_list);
            return SAME_TIME_ALL;
        case SAME_TIME_DEPARTURE:
            printf("Error: You have entered the same time of departure.\n");
            free_list_buses(&main_list);
            return SAME_TIME_DEPARTURE;
        case SAME_TIME_ARRIVE:
            printf("Error: You have entered the same time of arrival.\n");
            free_list_buses(&main_list);
            return SAME_TIME_ARRIVE;
        case DEPARTURE_LESS_ARRIVE:
            printf("Error: You have entered a departure time earlier than the arrival time for one bus.\n");
            free_list_buses(&main_list);
            return DEPARTURE_LESS_ARRIVE;
        case DEPARTURE_MORE_THAN_ARRIVING:
            printf("Error: You have entered a departure time later than or equal to some other arrival time.\n");
            free_list_buses(&main_list);
            return DEPARTURE_MORE_THAN_ARRIVING;
        default:
            break;
    }


    switch (check_coordination(&main_list))
    {
        case CHECK_NOT_SUCCESS:
            printf("Error: You have entered the same coordinates for two consecutive stops of the same bus.\n");
            free_list_buses(&main_list);
            return CHECK_NOT_SUCCESS;
        case CHECK_SUCCESS:
            break;
    }
    interactive(&main_list);
    free_list_buses(&main_list);
}