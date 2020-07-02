#ifndef STRSTACK_H
#define STRSTACK_H

#include <stdlib.h>
#include <string.h>

//Declare stack node
typedef struct strStackNodeStruct {
	char* val;
	struct strStackNodeStruct* next;
} sstrnode;

//Define strStack
typedef struct {
	//Begin acts as a buffer: all data
	//is placed after begin
	sstrnode* begin;
} strstack;

//Initialises strStack
strstack strstackInit();

//Pushes value val to the top of the stack
void sstr_push(strstack *s, char* val);

//Returns the string at the top of strstack s
//and then delete it
char* sstr_pop(strstack *s);

//Returns the string at the top of strstack s
char* sstr_top(strstack s);

#endif /* STRSTACK_H */
