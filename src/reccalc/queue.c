#include "queue.h"

queue queueInit() {
	queue temp;
	temp.begin = malloc(sizeof(qnode));
	temp.end = malloc(sizeof(qnode));

	temp.begin->val = INT_MIN;
	temp.begin->next = temp.end;
	temp.begin->prev = NULL;

	temp.end->val = INT_MIN;
	temp.end->next = NULL;
	temp.end->prev = temp.begin;
	return temp;
}

void enqueue(queue q, cplx val) {
	qnode* temp = malloc(sizeof(qnode));
	temp->val = val;
	temp->next = q.begin->next;
	temp->prev = q.begin;
	q.begin->next->prev = temp;
	q.begin->next = temp;
}

cplx dequeue(queue q) {
	if(q.begin->next == q.end) {
		//printf("shining eyes pierce veiled hands\n");
		return INT_MIN;
	}
	cplx ret = q.end->prev->val;
	qnode* temp = q.end->prev;
	q.end->prev = q.end->prev->prev;
	q.end->prev->next = q.end;
	free(temp);
	return ret;
}

cplx front(queue q) {
	if(q.begin->next == q.end) {
		return INT_MIN;
	}
	return q.end->prev->val;
}

cplx back(queue q) {
	if(q.begin->next == q.end) {
		return INT_MIN;
	}
	return q.begin->next->val;
}
