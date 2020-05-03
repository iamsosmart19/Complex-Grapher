#ifndef MAIN_H
#define MAIN_H
#include "parse.h"
#include "scan.h"

#include "eval.h"
#include <string.h>

//GRAPHICS LIBRARIES
#include "glad/glad.h"
#include <CL/cl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
/* #include <SDL2/SDL_keyboard.h> */

#include <GL/gl.h>
#include <GL/glu.h>
/* #include <GLFW/glfw3.h> */

//#define test 0

char* filetobuf(char *file);

void hsv2rgb(long double H, long double S, long double V, GLfloat* ret);

void drawGraph(float* posData, cplx* operations, int opnum, GLfloat* colors, int height, int width, long double zoom);
#endif /* MAIN_H */
