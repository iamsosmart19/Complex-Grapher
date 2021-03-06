#include "stack.h"

stack stackInit() {
	stack temp;
	temp.begin = malloc(sizeof(snode));
	temp.begin->val = -1;
	temp.begin->next = NULL;
	return temp;
}

void s_push(stack *s, cplx val) {
	snode* temp = malloc(sizeof(snode));
	temp->val = val;
	temp->next = (*s).begin->next;
	(*s).begin->next = temp;
}

cplx s_pop(stack *s) {
	if((*s).begin->next == NULL) {
		return -DBL_MAX - DBL_MAX * I;
	}
	cplx ret = (*s).begin->next->val;
	snode* temp = (*s).begin->next;
	(*s).begin->next = (*s).begin->next->next;
	free(temp);
	return ret;
}

cplx s_top(stack s) {
	if(s.begin->next == NULL) {
		return -DBL_MAX - DBL_MAX * I;
	}
	return s.begin->next->val;
}

