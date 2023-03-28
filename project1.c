/* 
 * File: project1.c
 * Author: Mariana Miranda - ist1102904
 * Description: Main project file
*/

#include "proj1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

line _lines[MaxLines];	/* Global array to store lines*/
stop _stops[MaxStops];	/* Global array to store stops*/

int _numLines;	/* Global variable to store the number of lines */
int _numStops;	/* Global variable to store the number of stops */


/* List with 'inverso' abbreviations */
static const char* abbreviations[] = {"inv", "inve", "inver", "invers", "inverso"};


int main() {

	while (command_handler()) {}

	return 0;
}


int command_handler() {
	char c = getchar();
	switch (c) {
	case 'p':
		add_list_stops(); /* Adds or lists stops */
		return 1;
	case 'c':
		add_list_lines(); /* Adds or lists lines */
		return 1;
	case 'l':
		list_connections(); /* Lists connections */
		return 1;
	case 'i':
		list_intersections(); /* Lists intersections */
		return 1;
	case 'q': /* Quits the program */
		return 0;
	default:
		return 1; /* Ignores unknown command */
	}
}


/* ----------- Read functions ----------- */

/* Reads a word from the standard input and returns the next character. 
   Handles the case where the name is in between double quotes */
char read_next_word(char word[]) {
    int i = 0;
    char c;

    while ((c = getchar()) != EOF && isspace(c)) {
        continue;
    }

    if (c == '\"') {
        while ((c = getchar()) != EOF && c != '\"') {
            word[i++] = c;
        }
        c = getchar();
    } else {
        word[i++] = c;
        while ((c = getchar()) != EOF && !isspace(c)) {
            word[i++] = c;
        }
    }
    word[i] = '\0';

    return c;
}


/* Reads a double from the standard input */
double read_next_double() {
	double num = 0.0;
	int is_negative = 0;
	char c;

	while (!isdigit(c = getchar()) && c != '-')
		;

	if (c == '-')
		is_negative = 1;
	else
		num = c - '0';

	while (isdigit(c = getchar()))
		num = num * 10.0 + (c - '0');

	if (c == '.') {
		double decimal = 0.1;
		while (isdigit(c = getchar())) {
			num += decimal * (c - '0');
			decimal *= 0.1;
		}
	}

	return is_negative ? -num : num;
}


/* ----------- 'c' command functions ----------- */

/* Function that handles the 'c' command according to the arguments */
void add_list_lines() {

	line line;
	int line_idx;
	char inv[LineDIM];

	char c, c2;
	if ((c = getchar()) == '\n') /* Input format: c */
		list_all_lines();
	else {
		c2 = read_next_word(line.name); /* Input format: c <line name> */
		line_idx = check_line(line.name);
		if (line_idx == DOES_NOT_EXIST) { /* Line does not exist */
			create_line(line.name);
		}
		else { /* Line exists*/
			if (c2 == '\n')
				list_line(line_idx);
			else if (c2 == ' ') { /* Input format: c <line name> <inv option> */
				read_next_word(inv);
				if (is_reverse(inv))
					list_reverse_line(line_idx);
				else
					printf("%s", INCORRECT_SORT_OPTION);
			}
		}
	}
}


/* Checks if a line with the given name exists */
int check_line(char name[]) {

	int i;
	for (i = 0; i < _numLines; i++) {
		if (!strcmp(_lines[i].name, name))
			return i;
	}
	return DOES_NOT_EXIST;
}


/* Creates new line */
void create_line(char name[]) {

	strcpy(_lines[_numLines].name, name);
	_lines[_numLines].number_stops = 0;
	_lines[_numLines].total_cost = 0;
	_lines[_numLines].total_duration = 0;
	_numLines++;
}


/* Lists line */
void list_line(int idx) {

	int i;
	/* if the line has no stops linked, nothing is printed */
	if (_lines[idx].start_stop.name[0] != '\0' && _lines[idx].end_stop.name[0] != '\0') {

		printf("%s, ", _lines[idx].start_stop.name);

		for (i = 1; i < _lines[idx].number_stops - 1; i++) {
			printf("%s, ", _stops[_lines[idx].stops_idx[i]].name);
		}
		printf("%s\n", _lines[idx].end_stop.name);
	}
}


/* Lists all lines */
void list_all_lines() {

	int i;
	for (i = 0; i < _numLines; i++) {
		/* tests if the line has any stops, if it has, prints start and end stop */
		if (_lines[i].start_stop.name[0] != '\0' && _lines[i].end_stop.name[0] != '\0') {
			printf("%s %s %s %d %.2f %.2f\n", _lines[i].name,
				   _lines[i].start_stop.name, _lines[i].end_stop.name,
				   _lines[i].number_stops, _lines[i].total_cost,
				   _lines[i].total_duration);
		} else {
			printf("%s %d %.2f %.2f\n", _lines[i].name, _lines[i].number_stops,
				   _lines[i].total_cost, _lines[i].total_duration);
		}
	}
}


/* Checks if the 'inverso' command is correct */
int is_reverse(char name[]) {

	int i;
	for (i = 0; i < 5; i++) {
		if (strcmp(name, abbreviations[i]) == 0)
			return 1;
	}
	return 0;
}


/* lists line in reverse */
void list_reverse_line(int idx) {

	int i;
	printf("%s, ", _lines[idx].end_stop.name);
	for (i = _lines[idx].number_stops - 2; i > 0; i--) {
		printf("%s, ", _stops[_lines[idx].stops_idx[i]].name);
	}
	printf("%s\n", _lines[idx].start_stop.name);
}


/* ----------- 'p' command functions ----------- */

/* Handles the 'p' command according to the arguments */
void add_list_stops() {

    stop stop;
    char c1, c2;
    double latitude, longitude;
    
    if((c1 = getchar()) == '\n') {  /* input format: p */
        list_all_stops();
    } else {
        c2 = read_next_word(stop.name);
        if (c2  == '\n') {   /* input format: p <stop_name> */
            if(check_stop(stop.name) == DOES_NOT_EXIST)
                printf("%s: %s", stop.name, NO_SUCH_STOP);
            else
                list_stop(stop.name);
        } else {   /* input format: p <stop_name> <latitude> <longitude> */
            if(check_stop(stop.name) != DOES_NOT_EXIST)
                printf("%s: %s", stop.name, STOP_ALREADY_EXISTS);
            else {
                latitude = read_next_double();
                longitude = read_next_double();
                create_stop(stop.name, latitude, longitude);
            }
        }
    }
}


/* Checks if a stop exists */
int check_stop(char name[]) {

    int i;
    for(i = 0; i < _numStops; i++) {
        if(!strcmp(_stops[i].name, name))
            return i;
    }
    return DOES_NOT_EXIST;
}


/* Creates a new stop */
void create_stop(char name[], double latitude, double longitude) {    

    strcpy(_stops[_numStops].name, name);
    _stops[_numStops].latitude = latitude;
    _stops[_numStops].longitude = longitude;
    _numStops++;
}


/* Checks if a stop name has correct dimension */
int valid_stop_name(char name[]) {

    if (strlen(name) > StopDIM)
        return 0;
    return 1;
}


/* Prints latitude and longitude of given stop */
void list_stop(char name[]) {

    int i;
    for(i = 0; i < _numStops; i++){
        if(strcmp(_stops[i].name, name) == 0)
            printf("%16.12f %16.12f\n", _stops[i].latitude, _stops[i].longitude);
    }
}


/* Prints all stops */
void list_all_stops() {

    int i;
    for(i = 0; i < _numStops; i++) {
        printf("%s: %16.12f %16.12f %d\n", _stops[i].name, _stops[i].latitude, 
            _stops[i].longitude, _stops[i].number_lines);
    }
}


/* ----------- 'l' command functions ----------- */

/* Function that handles the 'l' command
   Input format: l <line name> <start stop> <end stop> <cost> <duration> */
void list_connections() {

	connection connect;
	int line_idx, start_idx, end_idx;

	read_next_word(connect.line.name); /* Reads line name */
	line_idx = check_line(connect.line.name);
	if (line_idx == DOES_NOT_EXIST) {
		printf("%s: %s", connect.line.name, NO_SUCH_LINE);
		return;
	}

	read_next_word(connect.start_stop.name); /* Reads start stop */
	start_idx = check_stop(connect.start_stop.name);
	if (start_idx == DOES_NOT_EXIST) {
		printf("%s: %s", connect.start_stop.name, NO_SUCH_STOP);
		return;
	}

	read_next_word(connect.end_stop.name); /* Reads end stop */
	end_idx = check_stop(connect.end_stop.name);
	if (end_idx == DOES_NOT_EXIST) {
		printf("%s: %s", connect.end_stop.name, NO_SUCH_STOP);
		return;
	}

	connect.cost = read_next_double();	   /* Reads cost */
	connect.duration = read_next_double(); /* Reads duration */
	if (connect.cost < 0 || connect.duration < 0) {
		printf("%s", NEGATIVE_COST_DURATION);
		return;
	} else {
		if (add_connection_to_line(line_idx, connect.start_stop.name,
								   connect.end_stop.name)) {
			_lines[line_idx].total_cost += connect.cost;
			_lines[line_idx].total_duration += connect.duration;
		}
	}
}


/* Adds the connection to the line depending on the existing connections of the line */
int add_connection_to_line(int line_idx, char start_stop[], char end_stop[]) {
	int start_idx, end_idx;
	start_idx = check_stop(start_stop);
	end_idx = check_stop(end_stop);

	/* if the line has no stops, add the start and end stop to the line */
	if (_lines[line_idx].number_stops == 0) {
		_lines[line_idx].start_stop = _stops[start_idx];
		_lines[line_idx].end_stop = _stops[end_idx];

		/* update array of stops */
		_lines[line_idx].stops_idx[0] = start_idx;
		_lines[line_idx].stops_idx[1] = end_idx;
		_lines[line_idx].number_stops = 2;

		_stops[start_idx].number_lines++;
		_stops[end_idx].number_lines++;

		return 1;
	} else {
		if (strcmp(start_stop, _lines[line_idx].end_stop.name) == 0)
			add_to_end(line_idx, end_idx);

		else if (strcmp(end_stop, _lines[line_idx].start_stop.name) == 0)
			add_to_start(line_idx, start_idx);

		else {
			printf("%s", LINK_ERROR);
			return 0;
		}
		return 1;
	}
}


/* Adds the the 'end_idx' stop to the end of the array */
void add_to_end(int line_idx, int end_idx) {

	_lines[line_idx].end_stop = _stops[end_idx];

	_lines[line_idx].stops_idx[_lines[line_idx].number_stops] = end_idx;
	_lines[line_idx].number_stops++;

	/* check if it's a circular line */
	if (!strcmp(_lines[line_idx].start_stop.name, _stops[end_idx].name)) {
		_lines[line_idx].is_circular = 1;
		return;
	}

	_stops[end_idx].number_lines++;
}


/* Adds the the 'start_idx' stop to the beginning of the array */
void add_to_start(int line_idx, int start_idx) {

	int i;

	_lines[line_idx].start_stop = _stops[start_idx];
	/* move all the stops to add new in the beginning of the array*/
	for (i = _lines[line_idx].number_stops - 1; i >= 0; i--)
		_lines[line_idx].stops_idx[i + 1] = _lines[line_idx].stops_idx[i];

	_lines[line_idx].stops_idx[0] = start_idx;
	_lines[line_idx].number_stops++;

	/* check if it's a circular line */
	if (!strcmp(_lines[line_idx].end_stop.name, _stops[start_idx].name)) {
		_lines[line_idx].is_circular = 1;
		return;
	}

	_stops[start_idx].number_lines++;
}


/* ----------- i command ----------- */

/* Function that handles the 'i' command */
void list_intersections() {

	int i, j, k;

	/* goes through each stop in _stops */
	for (i = 0; i < _numStops; i++)	{
		int n_lines = _stops[i].number_lines;
		if (n_lines > 1) {

			int line_count = 0;
			char lines[MaxLines][LineDIM];
			/* goes through each line in _lines */
			for (j = 0; j < _numLines; j++) {
				/* for _line[j] checks if the stop of the outer loop matches */
				if (!(_lines[j].is_circular)) {
					/* if it isn't a circular line, it goes through the entire array */
					for (k = 0; k < _lines[j].number_stops; k++) {
						if (i == _lines[j].stops_idx[k]) {
							strcpy(lines[line_count], _lines[j].name);
							line_count++;
						}
					}
				} else {
					/* if it is a circular line, it goes through the
					entire array except the last stop */
					for (k = 0; k < _lines[j].number_stops - 1; k++) {
						if (i == _lines[j].stops_idx[k]) {
							strcpy(lines[line_count], _lines[j].name);
							line_count++;
						}
					}
				}
			}

			line_sort(lines, n_lines);

			printf("%s %d:", _stops[i].name, n_lines);
			for (j = 0; j < n_lines; j++) {
				printf(" %s", lines[j]);
			}
			printf("\n");
		}
	}
}


/* Bubble sort to sort lines in alphabetically order */
void line_sort(char arr[][LineDIM], int n) {

	int i, j;
	for (i = 0; i < n - 1; i++)	{
		for (j = 0; j < n - i - 1; j++) {
			if (strcmp(arr[j], arr[j + 1]) > 0) {
				char temp[LineDIM];
				strcpy(temp, arr[j]);
				strcpy(arr[j], arr[j + 1]);
				strcpy(arr[j + 1], temp);
			}
		}
	}
}