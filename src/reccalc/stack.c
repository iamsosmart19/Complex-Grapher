#include "stack.h"

stack stackInit() {
	stack temp;
	temp.begin = malloc(sizeof(snode));
	temp.begin->val = -1;
	temp.begin->next = NULL;
	return temp;
}

void push(stack s, cplx val) {
	snode* temp = malloc(sizeof(snode));
	temp->val = val;
	temp->next = s.begin->next;
	s.begin->next = temp;
}

cplx pop(stack s) {
	if(s.begin->next == NULL) {
		return DBL_MIN + DBL_MIN * I;
	}
	cplx ret = s.begin->next->val;
	snode* temp = s.begin->next;
	s.begin->next = s.begin->next->next;
	free(temp);
	return ret;
}

cplx top(stack s) {
	if(s.begin->next == NULL) {
		return DBL_MIN + DBL_MIN * I;
	}
	return s.begin->next->val;
}

