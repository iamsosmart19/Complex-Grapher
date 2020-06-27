#include "strstack.h"

strstack strstackInit() {
	strstack temp;
	temp.begin = malloc(sizeof(sstrnode));
	temp.begin->val = NULL;
	temp.begin->next = NULL;
	return temp;
}

void sstr_push(strstack s, const char* val) {
	sstrnode* temp = malloc(sizeof(sstrnode));
	temp->val = val;
	temp->next = s.begin->next;
	s.begin->next = temp;
}

char* sstr_pop(strstack s) {
	if(s.begin->next == NULL) {
		return NULL;
	}
	const char* ret = s.begin->next->val;
	sstrnode* temp = s.begin->next;
	s.begin->next = s.begin->next->next;
	free(temp);
	return ret;
}

char* sstr_top(strstack s) {
	if(s.begin->next == NULL) {
		return NULL;
	}
	return s.begin->next->val;
}

