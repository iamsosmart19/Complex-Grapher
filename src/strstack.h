#ifndef STRSTACK_H
#define STRSTACK_H

#include <stdlib.h>
#include <string.h>

typedef struct strStackNodeStruct {
	char* val;
	struct strStackNodeStruct* next;
} sstrnode;

typedef struct {
	sstrnode* begin;
} strstack;

strstack strstackInit();

void sstr_push(strstack *s, char* val);

char* sstr_pop(strstack *s);

char* sstr_top(strstack s);

#endif /* STRSTACK_H */
