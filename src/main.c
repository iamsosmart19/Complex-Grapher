#include "main.h"

int main(void) {
	/* FILE* sample = fopen("input.txt", "r"); */
	FILE* sample = fopen("input3.txt", "r");
	char function[1024];
	fgets(function, 1024, sample);
	fclose(sample);

	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	int count = 0;
	queue out = queueInit();
	result res = parse_string(function, &out, &count);
	cplx* output = (cplx*)malloc(count*sizeof(cplx));
	int cur = 0;

	printf("\nMAIN:\n"); 

	while(front(out) != INT_MIN) {
		/* if(cimag(front(out)) != -DBL_MAX && cimag(front(out)) != DBL_MAX) { */
		/* 	printf("%lf%+lfi\n", creal(front(out)), cimag(front(out))); */
		/* } */
		/* else if(cimag(front(out)) == -DBL_MAX) { */
		/* 	char table[6] = {'+', '-', '*', '/', '^', '_'}; */
		/* 	printf("OP: %c\n", table[(int)creal(front(out))]); */
		/* } */
		/* else { */
		/* 	printf("FUNC: %s\n", lookuptable[(int)creal(front(out))]); */
		/* } */
		output[cur++] = dequeue(&out);
	}
	printf("tknCnt: %d\n", count);

	/* for(int i = 0; i < count; i++) { */
	/* 	printf("[%d]: %lf%+lfi\n", i, creal(output[i]), cimag(output[i])); */
	/* } */

	cplx val = evalFunc(output, count, 1);
	printf("Eval: %lf+%lfi\n", creal(val), cimag(val));

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
