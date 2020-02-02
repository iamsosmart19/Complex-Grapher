#include "stack.h"
#include <stdio.h>
#include <complex.h>

int main(void) {
	stack s = stackInit();
	s_push(&s, 1);
	s_push(&s, 2);
	s_push(&s, 3);
	s_push(&s, 4 + I * 2);
	for(int i = 0; i < 4; i++) {
		printf("%lf%+lfi\n", creal(s_top(s)), cimag(s_top(s)));
		s_pop(&s);
	}
}
