#include "main.h"

int main(int argv, char* argc[]) {
	if(argv < 2) {
		fprintf(stderr, "Error: No file provided\n");
		return 1;
	}
	parseString(getString(argc[1]));
	return 0;
}
