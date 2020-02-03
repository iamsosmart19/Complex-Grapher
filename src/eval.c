#include "eval.h"

cplx evalFunc(cplx* arr, int tknCnt, cplx val) {

	char lookuptable[22][5] = {
		"SQRT", 
		"ROOT", 
		"LN", 
		"LOG", 
		"ABS", 
		"FLOOR", 
		"CEIL", 
		"ASIN", 
		"ACOS", 
		"ATAN", 
		"SINH", 
		"COSH", 
		"TANH", 
		"SECH", 
		"CSCH", 
		"COTH", 
		"SIN", 
		"COS", 
		"TAN", 
		"SEC", 
		"CSC", 
		"COT"
	};

	cplx ret = 0;
	for(int i = 0; i < tknCnt; i++) {
		if(cimag(arr[i]) == DBL_MAX) {
			printf("FUNC: %s\n", lookuptable[(int)creal(arr[i])]);
		}
		else if(cimag(arr[i]) == -DBL_MAX) {
			char table[6] = {'+', '-', '*', '/', '^', '_'};
			printf("OP: %c\n", table[(int)creal(arr[i])]);
		}
		else {
			printf("%lf%+lfi\n", creal(arr[i]), cimag(arr[i]));
		}
	}
	queue q = queueInit();
	stack s = stackInit();

	for(int i = 0; i < tknCnt; i++) {
		if(cimag(arr[i]) == DBL_MAX) {
			;
		}
		else if(cimag(arr[i]) == -DBL_MAX) {
			/* {'+', '-', '*', '/', '^', '_'}; */
			cplx oprnd1;
			cplx oprnd2;
			switch((int)creal(arr[i])) {
				case 0:
					s_push(&s, s_pop(&s) + s_pop(&s));
					break;

				case 1:
					s_push(&s, (0-s_pop(&s)) + s_pop(&s));
					break;

				case 2:
					s_push(&s, s_pop(&s) * s_pop(&s));
					break;

				case 3:
					s_push(&s, (1 / s_pop(&s)) * s_pop(&s));
					break;

				case 4:
					oprnd1 = s_pop(&s);
					oprnd2 = s_pop(&s);
					s_push(&s, pow(oprnd2, oprnd1));
					break;

				case 5:
					s_push(&s, - s_pop(&s));
					break;
			}
		}
		else {
			s_push(&s, arr[i]);
		}
	}
	ret = s_pop(&s);
	return ret;
}
