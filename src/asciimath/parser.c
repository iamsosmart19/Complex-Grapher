#include "parser.h"

//char* operations[];
//char* raw[];

int getString(const char* filename, char*** raw) {
	FILE* inputfile;
	lineNum = 0; //Holds number of lines
	int* lineLen; //int array; holds length of lines
	char c; //Temporary character holder

	inputfile = fopen(filename, "r");
	//Increments lineNum by 1 every time a '\n' is found; exits on EOF
	while(1) {
		c = fgetc(inputfile);
		if(c == EOF) {
			break;
		}
		if(c == '\n') {
			lineNum++;
		}
	}
	rewind(inputfile);

	lineLen = calloc(lineNum, sizeof(int));

	int i = 0;//counter for current position in lineLen
	while(1) {
		c = fgetc(inputfile);
		if(c == EOF) {
			break;
		}
		if(c == '\n') {
			i++;
			continue;
		}
		lineLen[i]++;
		//printf("number of things in line is now: %c: %d\n", c, lineLen[i]);
	}
	rewind(inputfile);
	
	//If there is nothing in the file, exit;
	if(lineLen[0] == 0) {
		fprintf(stderr, "Input file: %s is empty", inputfile);
		return 1;
	}
	
	//Sorts lineLen such that the largest element is in the front.
	qsort(lineLen, lineNum, sizeof(int), cmp);
	//Initialises raw with the number of lines and the space of the largest possible line
	raw = malloc(sizeof(char*) * lineNum);
	for(i = 0; i < lineNum; i++) {
		*(raw+i) = calloc(sizeof(char), (lineLen[0] + 1));
	}
	fflush(stderr);
	//fprintf(stderr, "ramen\n");

	for(i = 0; i < lineNum; i++) {
		fgets(*(raw+i), lineLen[0] + 1, inputfile);
	}

	for(i = 0; i < lineNum; i++) {
		printf("raw[%d]: |%s|\n", i, *(raw+i));
	}

	return 0;
}

int parseString(const char** raw, char** operations) {
	/*
	operations = malloc(sizeof(char*) * lineNum);
	for(int i = 0; i < lineNum; i++) {
		operations[i] = malloc(sizeof(char) * (sizeof(raw[0])));
	}
	*/

	//printf("%s\n", raw[0]);

	stack operators = stackInit();
	queue output = queueInit();
	for(int i = 0; i < lineNum; i++) {
		printf("%d: %s", i, (raw+i));
		for(int j = 0; j < sizeof(*(raw+i)); j++) {
			if(raw[i][j] == '\n') {
				break;
			}
			if(raw[i][j] > 47 && raw[i][j] < 58) {
				enqueue(output, raw[i][j]);
			}
			else {
				push(operators, raw[i][j]);
			}
		}
		while(top(operators) != -1) {
			enqueue(output, pop(operators));
		}
		while(back(output) != -1) {
			printf("%c", dequeue(output));
		}
		printf("\n");
	}
	return 0;
}
