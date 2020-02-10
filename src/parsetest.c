#include "main.h"

int main(void) {
	FILE* sample = fopen("input.txt", "r");
	/* FILE* sample = fopen("input3.txt", "r"); */
	char function[1024];
	fgets(function, 1024, sample);
	fclose(sample);

	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	queue out = queueInit();
	result res = parse_string(function, &out);
	printf("P1\n");

	if( res.nerrs ) {
		exit(0);
	}
	cplx* operations = (cplx*)malloc(256*sizeof(cplx));
	int count = 0;
	printf("P2\n");

	while(front(out) != INT_MIN) {
		operations[count++] = dequeue(&out);
	}
	operations = (cplx*)realloc(operations, count*sizeof(cplx));
	printf("P3\n");
	
	for(int i = 0; i < count; i++) {
		printf("%d: %Lf%+Lfi\n", i, creal(operations[i]), cimag(operations[i]));
	}
}
