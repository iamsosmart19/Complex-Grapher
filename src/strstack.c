#include "strstack.h"

//Initialises strStack
strstack strstackInit() {
	strstack temp;
	//Allocate sstrnode to beginning
	temp.begin = malloc(sizeof(sstrnode));
	//Set all values to NULL
	temp.begin->val = NULL;
	temp.begin->next = NULL;
	return temp;
}

//Pushes value val to the top of the stack
void sstr_push(strstack *s, char* val) {
	//Create new temporary stack
	sstrnode* temp = malloc(sizeof(sstrnode));

	//Allocate 8192 characters to temp->val
	temp->val = malloc(sizeof(char) * 8192);
	//Copy val into temp->val
	strcpy(temp->val, val);
	//Set value after temp to be the value after the beginning
	//Note: if there is nothing in the stack this is set to NULL
	temp->next = s->begin->next;
	//Set the next node after the beginning to be temp
	s->begin->next = temp;
}

//Returns the string at the top of strstack s
//and then delete it
char* sstr_pop(strstack *s) {
	//If there is nothing in the stack return NULL
	if(s->begin->next == NULL) {
		return NULL;
	}
	//Allocate last string to ret
	const char* ret = s->begin->next->val;
	//Temporary reference to first node
	sstrnode* temp = s->begin->next;
	//Set the node after the beginning to be the node
	//after the node after the beginning
	s->begin->next = s->begin->next->next;
	//Delete temp
	free(temp);
	return ret;
}

//Returns the string at the top of strstack s
char* sstr_top(strstack s) {
	//If there is nothing in the stack return NULL
	if(s.begin->next == NULL) {
		return NULL;
	}
	return s.begin->next->val;
}

