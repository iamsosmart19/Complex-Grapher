#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

typedef struct queueNodeStruct {
	int val;
	struct queueNodeStruct* next;
	struct queueNodeStruct* prev;
} qnode;

typedef struct queueStruct {
	qnode* begin;
	qnode* end;
} queue;

queue queueInit();

void enqueue(queue q, int val);

int dequeue(queue q);

int front(queue q);

int back(queue q);

#endif /* QUEUE_H */
