#ifndef MAIN_H
#define MAIN_H
#include "parse.h"
#include "scan.h"

#include "strstack.h"
#include "stack.h"
#include "complex.h"
#include "tgmath.h"
#include <string.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

char* funcToString(cplx* op, int opnum);

char* guide_function_table[30][3] = {
	{"a1+a2",
	"z+1, 2+i",
	"Adds a1 and a2 together"},
	{"a1-a2",
	"z-1,i-z",
	"Subtracts a2 from a1"},
	{"a1*a2",
	"z*i, 3*2",
	"Multiplies a1 with a2"},
	{"a1/a2",
	"z/9, 8*8",
	"Divides a1 by a2"},
	{"-a1",
	"-z, -(1+i)",
	"Makes a1 negative"},
	{"a1^a2",
	"z^2, e^(i*pi)",
	"Raises a1 to the power of a2"},
	{"sqrt(a1)",
	"sqrt(z), sqrt(e^z)",
	"Returns the square root of a1"},
	{"root(a1, a2)",
	"root(z, 3), root(z+i, 8)",
	"Returns a1 raised to the power of (1/a2) "},
	{"ln(a1)",
	"ln(e^z), ln(z+i)",
	"Takes the natural logarithm of a1"},
	{"log_(a1)(a2)",
	"log_(z)(1), log_(2+i)(z)",
	"Takes the log of a2 where the base is a1"},
	{"abs(a1)",
	"abs(z), abs(e^z)",
	"Gives the absolute value of a1"},
	{"|a1|",
	"|z|, |e^z|",
	"Gives the complex conjugate of a1"},
	{"re(a1)",
	"re(z), re(i*z)",
	"Gives the real part of a1"},
	{"im(a1)",
	"im(z), im(z*z)",
	"Gives the imaginary value of a1"},
	{"sin(a1)",
	"sin(z), sin(|z|)",
	"Gives the sine of a1"},
	{"cos(a1)",
	"cos(z), cos(sin(z))",
	"Gives the cosine of a1"},
	{"tan(a1)",
	"tan(z), tan(e^z)",
	"Gives the tangent of a1"},
	{"csc(a1)",
	"csc(z), csc(ln(z))",
	"Gives the cosecant of a1"},
	{"sec(a1)",
	"sec(z), sec(cos(z))",
	"Gives the secant of a1"},
	{"cot(a1)",
	"cot(z), cot(tan(z))",
	"Gives the cotanget of a1"},
	{"asin(a1)",
	"asin(z), asin(sin(z))",
	"Gives the arcsine of a1"},
	{"acos(a1)",
	"acos(z), acos(ln(z))",
	"Gives the arccosine of a1"},
	{"atan(a1)",
	"atan(z), atan(z*-i)",
	"Gives the arctangent of a1"},
	{"sinh(a1)",
	"sinh(z), sinh(z+6i)",
	"Gives the hyperbolic sine of a1"},
	{"cosh(a1)",
	"cosh(z), cosh(z^z)",
	"Gives the hyperbolic cosine of a1"},
	{"tanh(a1)",
	"tanh(z), tanh(re(z))",
	"Gives the hyperbolic tangent of a1"},
	{"csch(a1)",
	"csch(z), csch(z/i)",
	"Gives the hyperbolic cosecant of a1"},
	{"sech(a1)",
	"sech(z), sech(z^2-z)",
	"Gives the hyperbolic secant of a1"},
	{"coth(a1)",
	"coth(z), coth(z^8)",
	"Gives the hyperbolic cotangent of a1"}
};

char* guide_control_table[13][2] = {
	{"Spacebar",
	"The display screen recentres itself at 0+0i with the default zoom"},
	{"Comma",
	"Zooms out by 5%"},
	{"Less Than",
	"Zooms out by 20%"},
	{"Period",
	"Zooms in by 5%"},
	{"Greater than",
	"Zooms in by 20%"},
	{"Up arrow",
	"Moves display window up by a 1%"},
	{"Down arrow",
	"Moves display window down by 1%"},
	{"Left arrow",
	"Moves display window left by 1%"},
	{"Right arrow",
	"Moves display window right by 1%"},
	{"Shift + Up arrow",
	"Moves display window up by 1%"},
	{"Shift + Down arrow",
	"Moves display window down by 10%"},
	{"Shift + Left arrow",
	"Moves display window left by 10%"},
	{"Shift + Right arrow",
	"Moves display window right by 10%"}
};

//GTK TYPEDEFS

typedef GtkWidget gtkWindow;
typedef GtkWidget gtkFrame;
typedef GtkWidget gtkStack;
typedef GtkWidget gtkStackSwitcher;
typedef GtkWidget gtkStackSidebar;
typedef GtkWidget gtkButton;
typedef GtkWidget gtkEntry;
typedef GtkWidget gtkLabel;
typedef GtkWidget gtkBox;
typedef GtkWidget gtkGLArea;
typedef GtkWidget gtkFixed;

//GTK GL WRAPPERS

#define GLAREA_ERROR (glarea_error_quark ())

typedef enum {
  GLAREA_ERROR_SHADER_COMPILATION,
  GLAREA_ERROR_SHADER_LINK
} GlareaError;

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

typedef struct GlApplication_struct_decl {
	//FUNCTION
	cplx* operations;
	int opSize;
	char* funcString;

	//OPENCL
	ClProgram clProg;

	//OPENGL
	gtkWindow* display;
	gtkGLArea* area;
	int width, height;
	double interval;
	int n;

    size_t colorSize, posSize;
    size_t globalSize, localSize;
	float* posData;
	GLfloat* colors;

	float posOffset[2];
	double zoom;
	float zoomc;

	unsigned int gridOn;
	unsigned int shadOn;
	unsigned int axesOn;

	GLuint vao;
	GLuint triangleVBO;
	guint prog;

	//Controls
	gboolean shift_pressed;
} GlApplication;

//OPENCL FUNCTIONS

ClProgram create_cl_program(GlApplication* app, int width);

void write_to_clBuffer(GlApplication* app);

//GTK FUNCTIONS

static void on_entry_activate(GtkEntry* entry, GlApplication *app);

static void on_gl_realise(GtkGLArea *area, GlApplication *app);

static void on_gl_unrealise(GtkGLArea *area, GlApplication *app);

static gboolean render(GtkGLArea *area, GdkGLContext* context, GlApplication* app);

static void activate (GtkApplication *app, GlApplication* glMainApp);


void radio_toggled(GtkWidget* button, GlApplication* app);

void settings_toggled(GtkToggleButton* button, GlApplication* app);

static gboolean display_controls_press(GtkWidget* widget, GdkEventKey* event, GlApplication* app);

static gboolean display_controls_release(GtkWidget* widget, GdkEventKey* event, GlApplication* app);


static gboolean send_window_to_back(GtkWindow* window, GdkEvent *event, GlApplication* app);

static gboolean close_application(GtkWindow* window, GdkEvent* event, gtkWindow* display);

void quick_message (GtkWindow *parent, gchar *message);

//GL functions
static gboolean init_shader(guint** program);

static guint create_shader(int shader_type, const char *source, GError **error, guint *shader_out);

#endif /* MAIN_H */
