#include "parse.h"
#include "scan.h"

/*
int main(void) {
	FILE* sample = fopen("input.txt", "r");
	char function[1024];
	fgets(function, 1024, sample);
	fclose(sample);

	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	result res = parse_string(function);

	// Exit on failure if there were errors.
	return !!res.nerrs;
}
*/

int main(void) {
	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	result res = parse();

	// Exit on failure if there were errors.
	return !!res.nerrs;
}
