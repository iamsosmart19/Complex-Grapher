#ifndef STRSTACK_H
#define STRSTACK_H

#include <stdlib.h>
#include <string.h>

typedef struct stackNodeStruct {
	const char* val;
	struct stackNodeStruct* next;
} sstrnode;

typedef struct {
	sstrnode* begin;
} strstack;

strstack strstackInit();

void sstr_push(strstack s, const char* val);

char* sstr_pop(strstack s);

char* sstr_top(strstack s);

#endif /* STRSTACK_H */
