#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../stack.h"
#include "../queue.h"

//An array of strings; stores multiple functions if necessary
//char** operations;
//char** raw;

//Values for number of lines in file
int lineNum;

//extracts C strings from files into raw
int getString(const char* filename, char*** raw);

//converts raw into operationsc
int parseString(const char** raw, char** operations);

static int cmp(const void* a, const void* b) {
	return (*(int*)b - *(int*) a);
}

#endif /* PARSER_H */
