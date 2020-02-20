#include "complex.h"

typedef struct stackStruct {
	int cur;
	cplx arr[128];
} stack;

void initStack(stack *s) {
	s->cur = 0;
}

cplx s_pop(stack *s) {
	return s->arr[s->cur--];
}

void s_push(stack *s, cplx val) {
	s->arr[++(s->cur)] = val;
}
