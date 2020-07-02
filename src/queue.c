#include "queue.h"

queue queueInit() {
	queue temp;
	//Assign a qnode to the beginning and the end
	temp.begin = malloc(sizeof(qnode));
	temp.end = malloc(sizeof(qnode));

	//Initialise beginning to invalid values
	temp.begin->val = -DBL_MAX-DBL_MAX*I;
	//Set the node after the beginning to be the end
	temp.begin->next = temp.end;
	//and set the previous node to be NULL
	temp.begin->prev = NULL;

	//Initialise end to invalid values
	temp.end->val = -DBL_MAX-DBL_MAX*I;
	//Set the next node to be NULL
	temp.end->next = NULL;
	//and set the previous node to be the beginning
	temp.end->prev = temp.begin;

	//return created queue
	return temp;
}

void enqueue(queue *q, cplx val) {
	//Create a temporary qnode
	qnode* temp = malloc(sizeof(qnode));
	//Set the value to provided argument
	temp->val = val;
	//Set the next node to the first node after beginning
	//of queue q
	temp->next = (*q).begin->next;
	//Set the previous node to be the beginning of the queue
	temp->prev = (*q).begin;
	//Set the previous node for the node currently after begin
	//to temp
	(*q).begin->next->prev = temp;
	//Set the node after begin to be temp
	(*q).begin->next = temp;
}

cplx dequeue(queue* q) {
	//If the node after the first node is the end
	//of the queue, return an invalid value
	if((*q).begin->next == (*q).end) {
		return -DBL_MAX-DBL_MAX*I;
	}
	//Take the value of the node at the end
	cplx ret = (*q).end->prev->val;
	//Temporarily allocate last node in queue to temp
	qnode* temp = (*q).end->prev;
	//Set the node before the end to be the node
	//before the node before the end
	(*q).end->prev = (*q).end->prev->prev;
	//Set the node after the node behind the end to
	//be the end
	(*q).end->prev->next = (*q).end;
	//Delete the previously last node
	free(temp);
	return ret;
}

cplx front(queue q) {
	//If the node after the first node is the end
	//of the queue, return an invalid value
	if(q.begin->next == q.end) {
		return -DBL_MAX-DBL_MAX*I;
	}
	return q.end->prev->val;
}

cplx back(queue q) {
	//If the node after the first node is the end
	//of the queue, return an invalid value
	if(q.begin->next == q.end) {
		return -DBL_MAX-DBL_MAX*I;
	}
	return q.begin->next->val;
}
