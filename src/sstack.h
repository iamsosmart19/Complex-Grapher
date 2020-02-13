#ifndef SSTACK_H
#define SSTACK_H

#include <stdlib.h>
#include <string.h>

typedef struct stringStackNodeStruct {
	char val[512];
	struct stringStackNodeStruct* next;
} ssnode;

typedef struct {
	ssnode* begin;
} sstack;

sstack sstackInit();

void ss_push(sstack *s, char val[512]);

char* ss_pop(sstack *s);

char* ss_top(sstack s);

#endif /* SSTACK_H */
