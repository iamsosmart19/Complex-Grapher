#ifndef MAIN_H
#define MAIN_H
#include "parse.h"
#include "scan.h"

#include "eval.h"
#include <string.h>

//GRAPHICS LIBRARIES
/* #include "glad/glad.h" */
#include <CL/cl.h>
#include <gtk/gtk.h>
/* #include <SDL2/SDL_keyboard.h> */

#include <GL/gl.h>
#include <GL/glu.h>
/* #include <GLFW/glfw3.h> */

//#define test 0
typedef GtkWidget gtkWindow;
typedef GtkWidget gtkButton;
typedef GtkWidget gtkEntry;
typedef GtkWidget gtkBox;
typedef GtkWidget gtkGLArea;

char* filetobuf(char *file);

void hsv2rgb(long double H, long double S, long double V, GLfloat* ret);

void drawGraph(float* posData, cplx* operations, int opnum, GLfloat* colors, int height, int width, long double zoom);

//GTK FUNCTIONS

static void print_hello(GtkWidget *widget, gpointer data);

static void on_activate(GtkEntry* entry, gpointer user_data);

static void on_realise(GtkGLArea *area);

static gboolean render(GtkGLArea *area, GdkGLContext *context);

static void activate (GtkApplication *app, gpointer user_data);

#endif /* MAIN_H */
