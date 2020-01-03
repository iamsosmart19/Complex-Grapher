#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <complex.h>

typedef double complex cplx;

typedef struct queueNodeStruct {
	cplx val;
	struct queueNodeStruct* next;
	struct queueNodeStruct* prev;
} qnode;

typedef struct {
	qnode* begin;
	qnode* end;
} queue;

queue queueInit();

void enqueue(queue q, cplx val);

cplx dequeue(queue q);

cplx front(queue q);

cplx back(queue q);

#endif /* QUEUE_H */
