#include "sstack.h"

sstack sstackInit() {
	sstack temp;
	temp.begin = malloc(sizeof(ssnode));
	temp.begin->next = NULL;
	return temp;
}

void ss_push(sstack *s, char val[512]) {
	ssnode* temp = malloc(sizeof(ssnode));
	strcpy(temp->val, val);
	temp->next = (*s).begin->next;
	(*s).begin->next = temp;
}

char* ss_pop(sstack *s) {
	char* ret = (*s).begin->next->val;
	ssnode* temp = (*s).begin->next;
	(*s).begin->next = (*s).begin->next->next;
	free(temp);
	return ret;
}

char* ss_top(sstack s) {
	return s.begin->next->val;
}

