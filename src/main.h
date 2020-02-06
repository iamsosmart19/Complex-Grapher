#include "parse.h"
#include "scan.h"

#include "eval.h"

//GRAPHICS LIBRARIES
#include "glad/glad.h"
#include <GLFW/glfw3.h>

char* filetobuf(char *file);

void hsv2rgb(long double* H, long double* S, long double* V);
