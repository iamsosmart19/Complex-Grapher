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

void s_push(stack s, cplx val);

cplx s_pop(stack s);

cplx s_top(stack s);

#endif /* STACK_H */
