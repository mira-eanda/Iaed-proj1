/*
 * File: proj1.h
 * Author: Mariana Miranda - ist1102904
 * Description: Header file for project1.c 
 * Contains the constants, structs and function prototypes.
*/

#ifndef MAIN_H
#define MAIN_H

#define LineDIM 21	/* dimension of the name of the line */
#define StopDIM 51	/* dimension of the name of the stop */

#define MaxLines 200		 /* maximum number of lines */
#define MaxStops 10000		 /* maximum number of stops */
#define MaxConnections 30000 /* maximum number of connections */

/* 'c' command errors */
#define INCORRECT_SORT_OPTION "incorrect sort option.\n"

/* 'p' command errors */
#define STOP_ALREADY_EXISTS "stop already exists.\n"
#define NO_SUCH_STOP "no such stop.\n"

/* 'l' command errors */
#define NO_SUCH_LINE "no such line.\n"
#define LINK_ERROR "link cannot be associated with bus line.\n"
#define NEGATIVE_COST_DURATION "negative cost or duration.\n"

#define DOES_NOT_EXIST -1 /* return value for check_line and check_stop */

typedef struct {
    char name[StopDIM];
    double latitude;
    double longitude;
    int number_lines;
} stop;

typedef struct {
    char name[LineDIM];
    stop start_stop;
    stop end_stop;
    int stops_idx[MaxStops];
    int number_stops;
    double total_cost;
    double total_duration;
    int is_circular;
} line;

typedef struct {
    line line;
    stop start_stop;
    stop end_stop;
    double cost;
    double duration;
} connection;

int command_handler();

char read_next_word(char word[]);
double read_next_double();

void add_list_lines();
int check_line(char name[]);
void create_line(char name[]);
void list_line(int idx);
void list_all_lines();
int is_reverse(char name[]);
void list_reverse_line(int idx);

void add_list_stops();
int check_stop(char name[]);
void create_stop(char name[], double latitude, double longitude);
int valid_stop_name(char name[]);
void list_stop(char name[]);
void list_all_stops();

void list_connections();
int add_connection_to_line(int line_idx, char start_stop[], char end_stop[]);
void add_to_end(int line_idx, int end_idx);
void add_to_start(int line_idx, int start_idx);

void list_intersections();
void line_sort(char arr[][LineDIM], int n);

#endif