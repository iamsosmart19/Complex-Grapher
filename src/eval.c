#include "eval.h"

cplx evalFunc(cplx* arr, int tknCnt, cplx val) {

	static Handler funcTable[22] = {
		eval_SQRT, 
		eval_ROOT, 
		eval_LN, 
		eval_LOG, 
		eval_ABS, 
		eval_FLOOR, 
		eval_CEIL, 
		eval_ASIN, 
		eval_ACOS, 
		eval_ATAN, 
		eval_SINH, 
		eval_COSH, 
		eval_TANH, 
		eval_SECH, 
		eval_CSCH, 
		eval_COTH, 
		eval_SIN, 
		eval_COS, 
		eval_TAN, 
		eval_SEC, 
		eval_CSC, 
		eval_COT
	};

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
	/* for(int i = 0; i < tknCnt; i++) { */
	/* 	if(cimag(arr[i]) == DBL_MAX) { */
	/* 		printf("FUNC: %s\n", lookuptable[(int)creal(arr[i])]); */
	/* 	} */
	/* 	else if(cimag(arr[i]) == -DBL_MAX) { */
	/* 		char table[6] = {'+', '-', '*', '/', '^', '_'}; */
	/* 		printf("OP: %c\n", table[(int)creal(arr[i])]); */
	/* 	} */
	/* 	else { */
	/* 		printf("%lf%+lfi\n", creal(arr[i]), cimag(arr[i])); */
	/* 	} */
	/* } */
	stack s = stackInit();

	for(int i = 0; i < tknCnt; i++) {
		if(cimag(arr[i]) == DBL_MAX) {
			if(creal(arr[i]) == DBL_MAX) {
				s_push(&s, val);
			}
			else {
				funcTable[(int)creal(arr[i])](&s);
			}
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

void eval_SQRT(stack *s) {
	s_push(s, sqrt(s_pop(s)));
}

void eval_ROOT(stack *s){
	s_push(s, pow(s_pop(s), 1/s_pop(s)));
}
 
void eval_LN(stack *s){
	s_push(s, log(s_pop(s)));
}
 
void eval_LOG(stack *s){
	s_push(s, log(s_pop(s)) / log(s_pop(s)));
}
 
void eval_ABS(stack *s){
	s_push(s, fabs(s_pop(s)));
}
 
void eval_FLOOR(stack *s){
	s_push(s, (s_pop(s)));
}
 
void eval_CEIL(stack *s){
	s_push(s, (s_pop(s)));
}
 
void eval_ASIN(stack *s){
	s_push(s, asin(s_pop(s)));
}
 
void eval_ACOS(stack *s){
	s_push(s, acos(s_pop(s)));
}
 
void eval_ATAN(stack *s){
	s_push(s, atan(s_pop(s)));
}
 
void eval_SINH(stack *s){
	s_push(s, sinh(s_pop(s)));
}
 
void eval_COSH(stack *s){
	s_push(s, cosh(s_pop(s)));
}
 
void eval_TANH(stack *s){
	s_push(s, tanh(s_pop(s)));
}
 
void eval_SECH(stack *s){
	s_push(s, 1/cosh(s_pop(s)));
}
 
void eval_CSCH(stack *s){
	s_push(s, 1/sinh(s_pop(s)));
}
 
void eval_COTH(stack *s){
	s_push(s, 1/tanh(s_pop(s)));
}
 
void eval_SIN(stack *s){
	s_push(s, sin(s_pop(s)));
}
 
void eval_COS(stack *s){
	s_push(s, cos(s_pop(s)));
}
 
void eval_TAN(stack *s){
	s_push(s, tan(s_pop(s)));
}
 
void eval_SEC(stack *s){
	s_push(s, 1/cos(s_pop(s)));
}
 
void eval_CSC(stack *s){
	s_push(s, 1/sin(s_pop(s)));
}
 
void eval_COT(stack *s){
	s_push(s, 1/tan(s_pop(s)));
}

