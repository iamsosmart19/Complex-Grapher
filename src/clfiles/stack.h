#include "complex.h"

typedef struct stackStruct {
	int cur;
	cplx arr[128];
} stack;

inline void initStack(stack *s) {
	s->cur = 0;
}

inline cplx s_pop(stack *s) {
	return s->arr[s->cur--];
}

inline void s_push(stack *s, cplx val) {
	s->arr[++(s->cur)] = val;
}
