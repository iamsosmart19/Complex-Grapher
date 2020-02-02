#include "parse.h"
#include "scan.h"

/*
*/
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

int main(void) {
	FILE* sample = fopen("input.txt", "r");
	/* FILE* sample = fopen("input2.txt", "r"); */
	char function[1024];
	fgets(function, 1024, sample);
	fclose(sample);

	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	stack op = stackInit();
	queue out = queueInit();
	result res = parse_string(function, &op, &out);

	printf("\nMAIN:\n"); 

	while(front(out) != INT_MIN) {
		if(cimag(front(out)) != -DBL_MAX && cimag(front(out)) != DBL_MAX) {
			printf("%lf%+lfi\n", creal(front(out)), cimag(front(out)));
		}
		else if(cimag(front(out)) == -DBL_MAX) {
			char table[5] = {'+', '-', '*', '/', '^'};
			printf("OP: %c\n", table[(int)creal(front(out))]);
		}
		else {
			printf("FUNC: %s\n", lookuptable[(int)creal(front(out))]);
		}
		dequeue(out);
	}

	printf("STACK\n");
	while(s_top(op) != -DBL_MAX - DBL_MAX * I) {
		printf("%lf\n", creal(s_top(op)));
		s_pop(&op);
	}

	// Exit on failure if there were errors.
	return !!res.nerrs;
}
/*
*/

/*
int main(void) {
	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	result res = parse();

	// Exit on failure if there were errors.
	return !!res.nerrs;
}
*/
