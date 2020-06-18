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
#include <gio/gio.h>
/* #include <SDL2/SDL_keyboard.h> */

#include <epoxy/gl.h>
#include "glib-resources.h"
/* #include <GL/gl.h> */
/* #include <GL/glu.h> */
/* #include <GLFW/glfw3.h> */

//#define test 0
char* filetobuf(char *file);

void hsv2rgb(long double H, long double S, long double V, GLfloat* ret);

void drawGraph(float* posData, cplx* operations, int opnum, GLfloat* colors, int height, int width, long double zoom);

//OPENCL STRUCT
typedef struct ClProgram_struct_decl {
    cl_mem posBuffer;
	cl_mem opBuffer;
    cl_mem colorBuffer;

    cl_platform_id cpPlatform;        // OpenCL platform
    cl_device_id device_id;           // device ID
    cl_context context;               // context
    cl_command_queue queue;           // command queue
    cl_program program;               // program
    cl_kernel kernel;                 // kernel
} ClProgram;

//OPENCL FUNCTIONS

ClProgram create_cl_program();

//GTK TYPEDEFS

typedef GtkWidget gtkWindow;
typedef GtkWidget gtkButton;
typedef GtkWidget gtkEntry;
typedef GtkWidget gtkBox;
typedef GtkWidget gtkGLArea;

//GTK GL WRAPPERS

#define GLAREA_ERROR (glarea_error_quark ())

typedef enum {
  GLAREA_ERROR_SHADER_COMPILATION,
  GLAREA_ERROR_SHADER_LINK
} GlareaError;

typedef struct GlApplication_struct_decl {
	//FUNCTION
	cplx* operations;
	int opSize;

	//OPENCL
	ClProgram gpuProg;

	//OPENGL
	gtkGLArea* area;

	guint vao;
	guint prog;
} GlApplication;

//GTK FUNCTIONS

static void on_activate(GtkEntry* entry, gpointer user_data);

static void on_realise(GtkGLArea *area, GlApplication *app);

static void on_unrealise(GtkGLArea *area, GlApplication *app);

static gboolean render(GtkGLArea *area, GdkGLContext *context);

/* static void activate (GtkApplication *app, gpointer user_data); */
static void activate (GtkApplication *app, GlApplication* glMainApp);

//GL functions
static gboolean init_shader(guint** program);

static guint create_shader(int shader_type, const char *source, GError **error, guint *shader_out);

#endif /* MAIN_H */
