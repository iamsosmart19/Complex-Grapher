#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>

typedef struct stackNodeStruct {
	char val;
	struct stackNodeStruct* next;
} snode;

typedef struct stackStruct {
	snode* begin;
} stack;

stack stackInit();

void push(stack s, char val);

int pop(stack s);

int top(stack s);

#endif /* STACK_H */
