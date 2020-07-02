#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>

//Definition of a complex number
typedef double complex cplx;

//Node in a queue
typedef struct queueNodeStruct {
	cplx val;
	struct queueNodeStruct* next;
	struct queueNodeStruct* prev;
} qnode;

//Declaration of a queue
typedef struct {
	//Both of these essentiall act as a buffer
	//and data entered into the queue is stored
	//between these two
	qnode* begin;
	qnode* end;
} queue;

//Initialises queue
queue queueInit();

//Adds element to queue q with value val
void enqueue(queue* q, cplx val);

//Returns value from end of queue and removes it
cplx dequeue(queue* q);

//Returns value from the front of the queue
cplx front(queue q);

//Returns value from the back of the queue
cplx back(queue q);

#endif /* QUEUE_H */
