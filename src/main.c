#include "main.h"

int main(int argv, char* argc[]) {
	if(argv < 2) {
		fprintf(stderr, "Error: No file provided\n");
		return 1;
	}
	char **raw, **operations;
	getString(argc[1], &raw);
	printf("%d\n", raw);
	parseString(raw, operations);
	return 0;
}
