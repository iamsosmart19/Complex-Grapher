#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <float.h>
#include <complex.h>

typedef double complex cplx;

typedef struct stackNodeStruct {
	cplx val;
	struct stackNodeStruct* next;
} snode;

typedef struct {
	snode* begin;
} stack;

stack stackInit();

void push(stack s, cplx val);

cplx pop(stack s);

cplx top(stack s);

#endif /* STACK_H */
