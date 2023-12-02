
#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <ctype.h>


typedef enum status_codes
{
    SUCCESS,
    NOT_SUCCES,
    MEMORY_ALLOCATION_PROBLEM,
    INVALID,
    EMPTY_STRING
} status_codes;

typedef enum check_bus_time_arrive
{
    CHECK_SUCCESS,
    CHECK_NOT_SUCCESS
} check_bus_time_arrive;

typedef enum check_equal
{
    SAME_TIME_ARRIVE,
    SAME_TIME_DEPARTURE,
    SAME_TIME_ALL,
    SAME_TIME_ARRIVING_AND_DEPARTURE,
    DEPARTURE_LESS_ARRIVE,
    DEPARTURE_MORE_THAN_ARRIVING,
    SUCCESS_TIME
} same_time;

typedef enum input_status_codes
{
    I_SUCCESS,
    I_ERROR_FILE,
    I_INVALID_STRING,
    I_MEMORY_ALLOCATION_PROBLEM,
    I_EMPTY_FILE,
    I_EMPTY_STRING,
    I_SAME_COORD
} input_status_codes;

typedef enum status_arriving
{
    START,
    INTERMEDIATE,
    FINAL
} status_arriving;

typedef enum max_min
{
    MAX,
    MIN
} max_min;

typedef struct Bus
{
    char* bus_id;
    char* arrive;
    char* departure;
    double x;
    double y;
    status_arriving status;
} Bus;

typedef struct route_node
{
    Bus* bus;
    struct route_node* route_next;
} route_node;

typedef struct Route_list
{
    route_node* route_head;
} Route_list;

typedef struct main_node
{
    Route_list list_of_routes;
    struct main_node* main_next;
} main_node;

typedef struct Buses_list
{
    main_node* main_head;
} Buses_list;


route_node* create_route_node(Bus* bus)
{
    if(bus == NULL) return NULL;
    route_node* new_node = (route_node*)malloc(sizeof(route_node));
    if(new_node == NULL) return NULL;

    new_node->bus = bus;
    new_node->route_next = NULL;
    return new_node;
}

main_node* create_main_node()
{
    main_node* new_node = (main_node*) malloc(sizeof(main_node));
    if(new_node == NULL) return NULL;

    new_node->list_of_routes.route_head = NULL;
    new_node->main_next = NULL;
    return new_node;
}





#endif 