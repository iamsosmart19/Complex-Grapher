#include "parse.h"
#include "scan.h"

/*
*/
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

	printf("MAIN: top: %lf%+lfi\n", creal(front(out)), cimag(front(out)));

	while(front(out) != INT_MIN) {
		printf("%lf%+lfi\n", creal(front(out)), cimag(front(out)));
		dequeue(out);
	}

	printf("STACK\n");
	while(top(op) != DBL_MIN + DBL_MIN * I) {
		printf("%lf\n", creal(top(op)));
		pop(op);
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
