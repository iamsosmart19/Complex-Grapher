#include "main.h"
#ifdef test
	setenv("CUDA_CACHE_DISABLE", "1", 1);
#endif

int main(int argc, char* argv[]) {
	GtkApplication *app;
	GlApplication glMainApp;
	int app_status;

	FILE* sample = fopen("input.txt", "r");
	/* FILE* sample = fopen("input3.txt", "r"); */
	char function[1024];
	fgets(function, 1024, sample);
	printf("|%s|\n", function);
	fclose(sample);

	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	queue out = queueInit();
	result res = parse_string(function, &out);
	if( res.nerrs ) {
		exit(0);
	}
	printf("DB: 0\n");
	glMainApp.operations = (cplx*)malloc(128*sizeof(cplx));

	glMainApp.opSize = 0;
	while(front(out) != -DBL_MAX-DBL_MAX*I) {
		glMainApp.operations[glMainApp.opSize++] = dequeue(&out);
	}
	glMainApp.operations = (cplx*)realloc(glMainApp.operations, glMainApp.opSize*sizeof(cplx));
	printf("count: %d\n", glMainApp.opSize);

	glMainApp.clProg = create_cl_program();

	app = gtk_application_new("org.s1m7u.cplxgrapher", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), &glMainApp);
	app_status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return app_status;
}

static void activate (GtkApplication *app, GlApplication* glMainApp) {
	/* GlApplication glMainApp; */
	
	//Windows
	gtkWindow *window;
	gtkWindow *display;

	//CSS style elements
	GtkStyleContext *context;
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "entry.css", NULL);

	//GLarea
	GtkWidget* GLdispla_box;
	/* Glprogram shaderProgram; */

	//UI elements
	/* gtkButton *button; */
	/* GtkWidget *button_box; */

	gtkEntry *funcInput;
	GtkEntryBuffer *funcBuffer;
	gtkBox *funcBox;

	glMainApp->area = gtk_gl_area_new();
	gtk_gl_area_set_required_version(GTK_GL_AREA(glMainApp->area), 3, 3);
	gtk_widget_set_name(glMainApp->area, "display");
	g_signal_connect(glMainApp->area, "realize", G_CALLBACK (on_realise), glMainApp);
	g_signal_connect(glMainApp->area, "unrealize", G_CALLBACK (on_unrealise), glMainApp);
	g_signal_connect(glMainApp->area, "render", G_CALLBACK (render), NULL);
	gtk_widget_set_size_request(glMainApp->area, 600, 600);

	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Window");
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
	gtk_window_set_type_hint((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);

	display = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(display), "display");
	gtk_window_set_default_size(GTK_WINDOW(display), 600, 600);
	gtk_window_set_type_hint((GtkWindow*)display, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_keep_above(GTK_WINDOW(display), TRUE);

	gtk_widget_show_all(display);

	/* button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL); */
	/* gtk_container_add (GTK_CONTAINER (window), button_box); */

	/* button = gtk_button_new_with_label ("Hello World"); */
	/* g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL); */
	/* g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window); */
	/* gtk_container_add (GTK_CONTAINER (button_box), button); */

	funcBuffer = gtk_entry_buffer_new("", 0);

	funcBox = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add(GTK_CONTAINER(window), funcBox);

	GLdispla_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_size_request(GLdispla_box, 600, 600);
	gtk_container_add(GTK_CONTAINER(display), GLdispla_box);

	gtk_container_add(GTK_CONTAINER(GLdispla_box), glMainApp->area);

	funcInput = gtk_entry_new_with_buffer(funcBuffer);
	gtk_widget_set_name((funcInput), "funcIn");
	gtk_entry_set_max_width_chars(GTK_ENTRY(funcInput), 256);
	gtk_entry_set_placeholder_text(GTK_ENTRY(funcInput), "Enter function here");
	/* gtk_entry_set_has_frame(funcInput, 1); */
	/* char c = 'b'; */
	g_signal_connect(funcInput, "activate", G_CALLBACK(on_activate), NULL);

	context = gtk_widget_get_style_context(funcInput);
	gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	gtk_container_add(GTK_CONTAINER(funcBox), funcInput);

	gtk_widget_show_all(display);
	gtk_widget_show_all(window);
	gtk_window_move(GTK_WINDOW(window), 120, 200);
	gtk_window_move(GTK_WINDOW(display), 950, 200);
}

static void on_activate(GtkEntry* entry, gpointer user_data) {
	const char *name;
	name = gtk_entry_get_text(entry);

	g_print("\nHello %s+%c\n\n", name, 'b');
}

static void on_realise(GtkGLArea *area, GlApplication *app) {
	guint* program = &(app->prog);

	gtk_gl_area_make_current(GTK_GL_AREA(area));
	if(gtk_gl_area_get_error(GTK_GL_AREA(area)) != NULL) {
		printf("Could not get area\n");
		return;
	}
	GError *internal_error = NULL;

	init_shader(&program);

	/* init_buffer_objects(&internal_error); */
	/* if (internal_error != NULL) { */
	/* 	gtk_gl_area_set_error(area, internal_error); */
	/* 	g_error_free(internal_error); */
	/* 	return; */
	/* } */

	/* init_shaders(&internal_error); */
	/* if(internal_error != NULL) { */
	/* 	gtk_gl_area_set_error(area, internal_error); */
	/* 	g_error_free(internal_error); */
	/* 	return; */
	/* } */
	return;
}

static void on_unrealise(GtkGLArea *area, GlApplication *app) {
	/* gtkGLArea** area = &app->area; */
	guint* program = &(app->prog);

	gtk_gl_area_make_current(GTK_GL_AREA(area));

	if (gtk_gl_area_get_error(GTK_GL_AREA(area)) != NULL) {
		return;
	}

	if (app->vao != 0) {
		glDeleteVertexArrays(1, &app->vao);
	}
	if (program != 0) {
		glDeleteProgram(*program);
	}
}

static gboolean render (GtkGLArea *area, GdkGLContext *context) {
	glEnable(GL_PROGRAM_POINT_SIZE);
	/* gtk_gl_area_set_required_version(area, 3, 3); */
	// we can start by clearing the buffer
	/* printf("you spastic\n"); */
	glClearColor(0, 23, 23, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	return TRUE;
}

//GTK GL FUNCTIONS

static gboolean init_shader(guint** programExt) {
	GBytes* source;
	guint program = 0;
	guint vertex = 0, fragment = 0;
	GError* error;

	source = g_resources_lookup_data("/io/s1m7u/cplxgrapher/glfiles/vertShad.vert", 0, NULL);
	create_shader(GL_VERTEX_SHADER, g_bytes_get_data(source, NULL), &error, &vertex);
	g_bytes_unref(source);

	source = g_resources_lookup_data("/io/s1m7u/cplxgrapher/glfiles/fragShad.frag", 0, NULL);
	create_shader(GL_FRAGMENT_SHADER, g_bytes_get_data(source, NULL), &error, &fragment);
	g_bytes_unref(source);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	int status = 0;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int log_len = 0;
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &log_len);

		char *buffer = g_malloc (log_len + 1);
		glGetProgramInfoLog (program, log_len, NULL, buffer);

		/* g_set_error (error, GLAREA_ERROR, GLAREA_ERROR_SHADER_LINK, */
		/* 		"Linking failure in program: %s", buffer); */
		g_print("Linking failure in progam %s", buffer);

		g_free (buffer);

		glDeleteProgram (program);
		program = 0;

		goto out;
	}
out:

	if (vertex != 0) {
		glDeleteShader (vertex);
	}
	if (fragment != 0) {
		glDeleteShader (fragment);
	}

	if (program) {
		*programExt = &program;
	}

	return program != 0;
}

static guint create_shader(int shader_type, const char *source, GError **error, guint *shader_out) {
	guint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int status;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int log_len;
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_len);

		char *buffer = g_malloc (log_len + 1);
		glGetShaderInfoLog (shader, log_len, NULL, buffer);

		/* g_set_error(error, GLAREA_ERROR, GLAREA_ERROR_SHADER_COMPILATION, */
		/* 		"Compilation failure in %s shader: %s", */
		/* 		shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment", */
		/* 		buffer); */

		g_free (buffer);

		glDeleteShader (shader);
		shader = 0;
	}

	if (shader_out != NULL) {
		*shader_out = shader;
	}

	return shader != 0;
}

//UNFINISHED

ClProgram create_cl_program() {
	ClProgram clProg;

	int width = 800;
	int height = 1000;
	double interval = 0.002;
	int n = width * height;

    size_t colorSize = sizeof(GLfloat) * 3 * n;
    size_t posSize = sizeof(float) * 2 * n;

    size_t globalSize, localSize;
    cl_int err;

    // Number of work items in each local work group
    localSize = 512;

    // Number of total work items - localSize must be divisor
    globalSize = ceil(n/(float)localSize)*localSize;

    // Bind to platform
    err = clGetPlatformIDs(1, &(clProg.cpPlatform), NULL);

	if(err != CL_SUCCESS) {
		printf("error: %d\n", err);
	}

    // Get ID for the device
    err = clGetDeviceIDs(clProg.cpPlatform, CL_DEVICE_TYPE_GPU, 1, &(clProg.device_id), NULL);
	if(err != CL_SUCCESS) {
		printf("error: %d\n", err);
	}

    // Create a clProg.context  
    clProg.context = clCreateContext(0, 1, &(clProg.device_id), NULL, NULL, &err);
	if(err != CL_SUCCESS) {
		printf("error: %d\n", err);
	}
 
    // Create a command clProg.queue 
    clProg.queue = clCreateCommandQueueWithProperties(clProg.context, clProg.device_id, 0, &err);

	/* printf("BR: 0\n"); */

    // Create the compute clProg.program from the source buffer
	char kernelSource[16384];
	memset(kernelSource, '\0', 16384);
	FILE* kernelFile;
#ifdef test
		kernelFile = fopen("ctest.cl", "r");
#else 
		kernelFile = fopen("graph.cl", "r");
#endif
	char kTemp[512];
	while(fgets(kTemp, 512, kernelFile) != NULL) {
		strcat(kernelSource, kTemp);
	}
	fclose(kernelFile);
	const char* kSrcPtr = kernelSource;
    clProg.program = clCreateProgramWithSource(clProg.context, 1, (const char **)&kSrcPtr, NULL, &err);

	if(err != CL_SUCCESS) {
		printf("clProg.program build failed\n");
		printf("error: %d\n", err);
	}
 
    // Build the clProg.program executable 
    err = clBuildProgram(clProg.program, 0, NULL, NULL, NULL, NULL);
	if(err != CL_SUCCESS) {
		printf("build error: %d\n", err);
		size_t errorSize;
		clGetProgramBuildInfo(clProg.program, clProg.device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &errorSize);
		
		char* log = malloc(errorSize);
		clGetProgramBuildInfo(clProg.program, clProg.device_id, CL_PROGRAM_BUILD_LOG, errorSize, log, NULL);

		printf("|%s|\n", log);
	}

    // Create the compute clProg.kernel in the clProg.program we wish to run
    clProg.kernel = clCreateKernel(clProg.program, "graph", &err);
	if(err != CL_SUCCESS) {
		printf("link error: %d\n", err);
	}

	/* printf("count*sizeof(cplx): %ld\n", count * sizeof(cplx)); */
 
    // Create the input and output arrays in device memory for our calculation
    clProg.posBuffer = clCreateBuffer(clProg.context, CL_MEM_READ_ONLY, posSize, NULL, NULL);
	clProg.opBuffer = clCreateBuffer(clProg.context, CL_MEM_READ_ONLY, 256*sizeof(cplx), NULL, NULL);

    clProg.colorBuffer = clCreateBuffer(clProg.context, CL_MEM_WRITE_ONLY, colorSize, NULL, NULL);
 
	return clProg;
}
	/*-----------------------------------------------------------
	 * Code after this point should be pulled into when entry is typed into
	 */
//23 comment


/*     // Write our data set into the input array in device memory */
/*     err = clEnclProg.queueWriteBuffer(clProg.queue, clProg.posBuffer, CL_TRUE, 0, posSize, posData, 0, NULL, NULL); */
/*     err |= clEnclProg.queueWriteBuffer(clProg.queue, clProg.opBuffer, CL_TRUE, 0, count*sizeof(cplx), operations, 0, NULL, NULL); */

/*     // Set the arguments to our compute clProg.kernel */
/*     err  = clSetKernelArg(clProg.kernel, 0, sizeof(cl_mem), &(clProg.posBuffer)); */
/*     err |= clSetKernelArg(clProg.kernel, 1, sizeof(cl_mem), &(clProg.colorBuffer)); */
/*     err |= clSetKernelArg(clProg.kernel, 2, sizeof(cl_mem), &(clProg.opBuffer)); */
/*     err |= clSetKernelArg(clProg.kernel, 3, sizeof(int), &count); */
/*     err |= clSetKernelArg(clProg.kernel, 4, sizeof(double), &zoom); */
/*     err |= clSetKernelArg(clProg.kernel, 5, sizeof(float), &zoomc); */
/*     err |= clSetKernelArg(clProg.kernel, 6, sizeof(float)*2, &posOffset); */
/*     err |= clSetKernelArg(clProg.kernel, 7, sizeof(unsigned int), &n); */
 
/*     // Execute the clProg.kernel over the entire range of the data set */  
/*     err = clEnclProg.queueNDRangeKernel(clProg.queue, clProg.kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL); */
 
/*     // Wait for the command clProg.queue to get serviced before reading back results */
/*     clFinish(clProg.queue); */
 
/*     // Read the results from the device */
/* }; */

//457 lines
/* 	//SDL */
/* 	//The window we'll be rendering to */
/* 	/1* SDL_StartTextInput(); *1/ */

/* 	//SDL OpenGL code */
/* 	SDL_GLContext displayContext; */

/* 	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); */
/*     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); */
/* 	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); */
/*     SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); */

/* 	SDL_Window* display = SDL_CreateWindow("Display", 1050, 200, 600, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN); */
/* 	int displayID = SDL_GetWindowID(display); */

/* 	displayContext = SDL_GL_CreateContext(display); */

/* 	SDL_GL_MakeCurrent(display, displayContext); */

/* 	SDL_GL_SetSwapInterval(1); */

/* 	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) */
/* 	{ */
/* 		return -1; */
/* 	} */

/* 	SDL_GL_SwapWindow(display); */
/* 	SDL_GL_MakeCurrent(display, displayContext); */

/* 	glViewport(0, 0, 800, 600); */

/* 	//Points */
/* 	glEnable(GL_PROGRAM_POINT_SIZE); */

/* 	//VBO */
/* 	GLuint shaderProgram; */
/* 	GLuint vboVert, vboFrag; */
/* 	char *vertSource, *fragSource; */

/* 	vertSource = filetobuf("vertShad.vert"); */
/* 	vboVert = glCreateShader(GL_VERTEX_SHADER); */
/* 	glShaderSource(vboVert, 1, (const GLchar**)&vertSource, 0); */
/* 	glCompileShader(vboVert); */

/* 	GLint vertSuccess; */
/* 	glGetShaderiv(vboVert, GL_COMPILE_STATUS, &vertSuccess); */
/* 	if(vertSuccess != GL_TRUE) { */
/* 		printf("Error: compile error for shader vboVert: \n"); */
/* 	} */

/* 	fragSource = filetobuf("fragShad.frag"); */
/* 	vboFrag = glCreateShader(GL_FRAGMENT_SHADER); */
/* 	glShaderSource(vboFrag, 1, (const GLchar**)&fragSource, 0); */
/* 	glCompileShader(vboFrag); */

/* 	shaderProgram = glCreateProgram(); */
/* 	glAttachShader(shaderProgram, vboVert); */
/* 	glAttachShader(shaderProgram, vboFrag); */

/* 	glLinkProgram(shaderProgram); */
/* 	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &vertSuccess); */
/* 	if(vertSuccess != GL_TRUE) { */
/* 		printf("Error: compile error for program shaderProgram: \n"); */
/* 	} */
/* 	glUseProgram(shaderProgram); */

/* 	GLuint vao; */
/* 	GLuint triangleVBO; */
/* 	float* posData; */
/* 	float posOffset[2] = {0, 0}; */
/* 	GLfloat* colors; */
/* 	/1* int width = 1600; *1/ */
/* 	/1* int height = 2000; *1/ */
/* 	/1* double interval = 0.001; *1/ */
/* 	int width = 800; */
/* 	int height = 1000; */
/* 	double interval = 0.002; */
/* 	int n = width * height; */
/* 	double zoom = 10; */
/* 	/1* float zoomc = zoom > 1.0 ? (1.0/4.0)*((1.0/2.0)+(1.0/log(zoom+(cpowl(M_E, 2.0/3.0)+1.0)-1.0))) : 0.5; *1/ */
/* 	float zoomc = 0.001; */
/* 	printf("zoomc: %f\n", zoomc); */
/* 	glPointSize(interval * 500); */

/*     size_t posSize = sizeof(float) * 2 * n; */
/*     size_t colorSize = sizeof(GLfloat) * 3 * n; */

/* 	posData = malloc(posSize); */
/* 	colors = malloc(colorSize); */

/* 	for(int i = 0; i < width; i++) { */
/* 		for(int j = 0; j < height*2; j+=2) { */
/* 			posData[i*height*2+j] = -1.0 + i*interval; */
/* 			posData[i*height*2+j+1] = -1.0 + (j/2)*interval; */
/* 		} */
/* 	} */

/* 	//OpenCL code; */

/*     // Device input buffers */
/*     cl_mem posBuffer; */
/* 	cl_mem opBuffer; */
/*     // Device output buffer */
/*     cl_mem colorBuffer; */

/*     cl_platform_id cpPlatform;        // OpenCL platform */
/*     cl_device_id device_id;           // device ID */
/*     cl_context context;               // context */
/*     cl_command_queue queue;           // command queue */
/*     cl_program program;               // program */
/*     cl_kernel kernel;                 // kernel */

/*     size_t globalSize, localSize; */
/*     cl_int err; */

/*     // Number of work items in each local work group */
/*     localSize = 512; */

/*     // Number of total work items - localSize must be divisor */
/*     globalSize = ceil(n/(float)localSize)*localSize; */

/*     // Bind to platform */
/*     err = clGetPlatformIDs(1, &cpPlatform, NULL); */

/* 	if(err != CL_SUCCESS) { */
/* 		printf("error: %d\n", err); */
/* 	} */

/*     // Get ID for the device */
/*     err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL); */
/* 	if(err != CL_SUCCESS) { */
/* 		printf("error: %d\n", err); */
/* 	} */

/*     // Create a context */  
/*     context = clCreateContext(0, 1, &device_id, NULL, NULL, &err); */
/* 	if(err != CL_SUCCESS) { */
/* 		printf("error: %d\n", err); */
/* 	} */
 
/*     // Create a command queue */ 
/*     queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err); */

/* 	/1* printf("BR: 0\n"); *1/ */

/*     // Create the compute program from the source buffer */
/* 	char kernelSource[16384]; */
/* 	memset(kernelSource, '\0', 16384); */
/* 	FILE* kernelFile; */
/* #ifdef test */
/* 		kernelFile = fopen("ctest.cl", "r"); */
/* #else */ 
/* 		kernelFile = fopen("graph.cl", "r"); */
/* #endif */
/* 	char kTemp[512]; */
/* 	while(fgets(kTemp, 512, kernelFile) != NULL) { */
/* 		strcat(kernelSource, kTemp); */
/* 	} */
/* 	fclose(kernelFile); */
/* 	/1* printf("|%s|", kernelSource); *1/ */
/* 	/1* printf("\n\n"); *1/ */


/* 	const char* kSrcPtr = kernelSource; */
/*     program = clCreateProgramWithSource(context, 1, (const char **)&kSrcPtr, NULL, &err); */

/* 	if(err != CL_SUCCESS) { */
/* 		printf("program build failed\n"); */
/* 		printf("error: %d\n", err); */
/* 	} */
 
/*     // Build the program executable */ 
/*     err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL); */
/* 	if(err != CL_SUCCESS) { */
/* 		printf("build error: %d\n", err); */
/* 		size_t errorSize; */
/* 		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &errorSize); */
		
/* 		char* log = malloc(errorSize); */
/* 		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, errorSize, log, NULL); */

/* 		printf("|%s|\n", log); */
/* 	} */

/*     // Create the compute kernel in the program we wish to run */
/*     kernel = clCreateKernel(program, "graph", &err); */
/* 	if(err != CL_SUCCESS) { */
/* 		printf("link error: %d\n", err); */
/* 	} */

/* 	printf("count*sizeof(cplx): %ld\n", count * sizeof(cplx)); */
 
/*     // Create the input and output arrays in device memory for our calculation */
/*     posBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, posSize, NULL, NULL); */
/* 	opBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, count*sizeof(cplx), NULL, NULL); */

/*     colorBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, colorSize, NULL, NULL); */
 
/*     // Write our data set into the input array in device memory */
/*     err = clEnqueueWriteBuffer(queue, posBuffer, CL_TRUE, 0, posSize, posData, 0, NULL, NULL); */
/*     err |= clEnqueueWriteBuffer(queue, opBuffer, CL_TRUE, 0, count*sizeof(cplx), operations, 0, NULL, NULL); */

/*     // Set the arguments to our compute kernel */
/*     err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &posBuffer); */
/*     err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &colorBuffer); */
/*     err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &opBuffer); */
/*     err |= clSetKernelArg(kernel, 3, sizeof(int), &count); */
/*     err |= clSetKernelArg(kernel, 4, sizeof(double), &zoom); */
/*     err |= clSetKernelArg(kernel, 5, sizeof(float), &zoomc); */
/*     err |= clSetKernelArg(kernel, 6, sizeof(float)*2, &posOffset); */
/*     err |= clSetKernelArg(kernel, 7, sizeof(unsigned int), &n); */
 
/*     // Execute the kernel over the entire range of the data set */  
/*     err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL); */
 
/*     // Wait for the command queue to get serviced before reading back results */
/*     clFinish(queue); */
 
/*     // Read the results from the device */
/*     clEnqueueReadBuffer(queue, colorBuffer, CL_TRUE, 0, colorSize, colors, 0, NULL, NULL); */

/* #ifdef test */
/* 	cplx ret = -1.23 + 2.01*I; */
/* 	cplx ctestTemp; */
/* 	for(int i = 0; i < 26 * 3; i += 3) { */
/* 		switch(i/3) { */
/* 			case 0: */
/* 				ctestTemp = ret + ret; */
/* 				break; */

/* 			case 1: */
/* 				ctestTemp = ret - ret; */
/* 				break; */

/* 			case 2: */
/* 				ctestTemp = ret * ret; */
/* 				break; */

/* 			case 3: */
/* 				ctestTemp = ret / (12.3 - 3 * I); */
/* 				break; */

/* 			case 4: */
/* 				ctestTemp = creal(ret); */
/* 				break; */

/* 			case 5: */
/* 				ctestTemp = cimag(ret); */
/* 				break; */

/* 			case 6: */
/* 				ctestTemp = conj(ret); */
/* 				break; */

/* 			case 7: */
/* 				ctestTemp = carg(ret); */
/* 				break; */

/* 			case 8: */
/* 				ctestTemp = cproj(ret); */
/* 				break; */

/* 			case 9: */
/* 				ctestTemp = cabs(ret); */
/* 				break; */

/* 			case 10: */
/* 				ctestTemp = cexp(ret); */
/* 				break; */

/* 			case 11: */
/* 				ctestTemp = clog(ret); */
/* 				break; */

/* 			case 12: */
/* 				ctestTemp = cpow(ret, ret); */
/* 				break; */

/* 			case 13: */
/* 				ctestTemp = csqrt(ret); */
/* 				break; */

/* 			case 14: */
/* 				ctestTemp = csin(ret); */
/* 				break; */

/* 			case 15: */
/* 				ctestTemp = ccos(ret); */
/* 				break; */

/* 			case 16: */
/* 				ctestTemp = ctan(ret); */
/* 				break; */

/* 			case 17: */
/* 				ctestTemp = csinh(ret); */
/* 				break; */

/* 			case 18: */
/* 				ctestTemp = ccosh(ret); */
/* 				break; */

/* 			case 19: */
/* 				ctestTemp = ctanh(ret); */
/* 				break; */

/* 			case 20: */
/* 				ctestTemp = casinh(ret); */
/* 				break; */

/* 			case 21: */
/* 				ctestTemp = cacosh(ret); */
/* 				break; */

/* 			case 22: */
/* 				ctestTemp = catanh(ret); */
/* 				break; */

/* 			case 23: */
/* 				ctestTemp = casin(ret); */
/* 				break; */

/* 			case 24: */
/* 				ctestTemp = cacos(ret); */
/* 				break; */

/* 			case 25: */
/* 				ctestTemp = catan(ret); */
/* 					break; */
/* 		} */

/* 		printf("%d SHADER: %f%+fi\n", i/3, colors[i] , colors[i+1] ); */
/* 		printf("%d COMPLX: %lf%+lfi\n", i/3, creal(ctestTemp) , cimag(ctestTemp)); */
/* 		printf("%d: %f%+fi\n\n", i/3, colors[i] - creal(ctestTemp) , colors[i+1] - cimag(ctestTemp)); */
/* 	} */
/* #else */
/* 	printf("%f\n", posData[0]); */
/* 	printf("%f\n", posData[1]); */
/* 	printf("%f\n", colors[0]); */
/* 	printf("%f\n", colors[1]); */
/* 	printf("%f\n", colors[2]); */
/* 	printf("%f\n", colors[3]); */
/* 	printf("%f\n", colors[4]); */
/* 	printf("%f\n", colors[5]); */
/* 	printf("%f\n", colors[3*n-3]); */
/* 	printf("%f\n", colors[3*n-2]); */
/* 	printf("%f\n", colors[3*n-1]); */
/* #endif */

/* 	/1* for(int i = 0; i < width; i++) { *1/ */
/* 	/1* 	for(int j = 0; j < height*2; j+=2) { *1/ */
/* 			/1* printf("%1.2f, %1.2f  ", posData[i*height+j], posData[i*height+j+1]); *1/ */
/* 			/1* printf("%1.3f,   ", posData[i*height*2+j]); *1/ */
/* 			/1* printf("%1.3f,   ", posData[i*height*2+j+1]); *1/ */
/* 		/1* } *1/ */
/* 		/1* printf("\n"); *1/ */
/* 	/1* } *1/ */

/* 	/1* for(int i = 0; i < width * 4; i++) { *1/ */
/* 	/1* 	printf("(%d) %1.3f\n", i, posData[i]); *1/ */
/* 	/1* } *1/ */

/* 	glGenVertexArrays(1, &vao); */
/* 	glBindVertexArray(vao); */

/* 	glGenBuffers(1, &triangleVBO); */
/* 	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO); */
/* 	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * width * height + sizeof(GLfloat) * 3 * width * height, NULL, GL_DYNAMIC_DRAW); */

/* 	/1* printf("GLfloat: %ld, float: %ld\n", sizeof(GLfloat), sizeof(float)); *1/ */

/* 	//Position */
/* 	GLint posAttrb = glGetAttribLocation(shaderProgram, "position"); */

/* 	glVertexAttribPointer(posAttrb, 2, GL_FLOAT, GL_FALSE, 0, 0); */

/* 	glEnableVertexAttribArray(posAttrb); */

/* 	//Color */
/* 	printf("colors size: %ld\n", sizeof(GLfloat) * 3 * width * height); */

/* 	GLint colorAttrb = glGetAttribLocation(shaderProgram, "color"); */

/* 	glVertexAttribPointer(colorAttrb, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 2 * width * height)); */

/* 	glEnableVertexAttribArray(colorAttrb); */

/* 	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * width * height, posData); */

/* 	/1* glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); *1/ */

/* 	glOrtho(-300, 300, -300, 300, 0, 0); */

/* 	int graphDrawn = 0; */

/* 	while(!closeApp) { */
/* 		//input */

/* 		//render if graph not drawn, and then set drawn to true */
		/* if(!graphDrawn) { */
		/* 	err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &posBuffer); */
		/* 	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &colorBuffer); */
		/* 	err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &opBuffer); */
		/* 	err |= clSetKernelArg(kernel, 3, sizeof(int), &count); */
		/* 	err |= clSetKernelArg(kernel, 4, sizeof(double), &zoom); */
		/* 	err |= clSetKernelArg(kernel, 5, sizeof(float), &zoomc); */
		/* 	err |= clSetKernelArg(kernel, 6, sizeof(float)*2, &posOffset); */
		/* 	err |= clSetKernelArg(kernel, 7, sizeof(unsigned int), &n); */
		 
		/* 	// Execute the kernel over the entire range of the data set */  
		/* 	err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL); */
		 
		/* 	// Wait for the command queue to get serviced before reading back results */
		/* 	clFinish(queue); */
		 
		/* 	// Read the results from the device */
		/* 	clEnqueueReadBuffer(queue, colorBuffer, CL_TRUE, 0, colorSize, colors, 0, NULL, NULL); */
		/* 	/1* drawGraph(posData, operations, count, colors, height, width, zoom); *1/ */
		/* 	glBufferSubData(GL_ARRAY_BUFFER, posSize, colorSize, colors); */
		/* 	graphDrawn = 1; */
		/* } */

		/* //Draw Textbox */
		/* /1* SDL_RenderCopy(SDLMainWindowRenderer, textBoxTexture, NULL, &dstrect); *1/ */
		/* /1* SDL_RenderPresent(SDLMainWindowRenderer); *1/ */
		
		/* /1* //Events *1/ */
		/* /1* SDL_UpdateWindowSurface(SDLwindow); *1/ */
		/* /1* SDL_RenderPresent(SDLMainWindowRenderer); *1/ */
		/* /1* /2* glfwSwapBuffers(display); *2/ *1/ */

		/* /1* SDL_GL_MakeCurrent(display, displayContext); *1/ */

		/* /1* /2* SDL_SetRenderDrawColor( SDLMainWindowRenderer, 0x12, 0x67, 0xBF, 0xFF ); *2/ *1/ */
		/* /1* /2* SDL_RenderClear( SDLMainWindowRenderer); *2/ *1/ */

		/* /1* //draw VBO *1/ */
		/* /1* glBindVertexArray(vao); *1/ */
		/* /1* glDrawArrays(GL_POINTS, 0, width * height); *1/ */

		/* /1* SDL_GL_SwapWindow(display); *1/ */

		/* /1* printf(""); *1/ */
	/* } */

	/* /1* glfwTerminate(); *1/ */

    /* // release OpenCL resources */
    /* clReleaseMemObject(posBuffer); */
	/* clReleaseMemObject(opBuffer); */
    /* clReleaseMemObject(colorBuffer); */
    /* clReleaseProgram(program); */
    /* clReleaseKernel(kernel); */
    /* clReleaseCommandQueue(queue); */
    /* clReleaseContext(context); */

	/* // Exit on failure if there were errors. */
/* } */

char* filetobuf(char *file) {
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "r"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

void hsv2rgb(long double H, long double S, long double V, GLfloat* ret) {
	if(H < -M_PI || H > M_PI + 0.001) {
		ret[0] = 0;
		ret[1] = 0;
		ret[2] = 0;
		return;
	}
		
	if(S < 0 || S > 1) {
		ret[0] = 0;
		ret[1] = 0;
		ret[2] = 0;
		return;
	}

	if(V < 0 || V > 1) {
		ret[0] = 0;
		ret[1] = 0;
		ret[2] = 0;
		return;
	}
	long double C = S * V;
	long double Hs = ((H * 180)/M_PI) > 0 ? (H*180)/M_PI: 360 + (H*180)/M_PI;
	long double X = C * (1 - fabsl(fmodl(Hs/60.0, 2) - 1));
	/* printf("C: %llf\n\n", (long double)252.0 / 60.0); */
	/* printf("C: %llf\n\n", (long double)((H * 180) / M_PI )); */
	long double m = V - C;
	long double Rs, Gs, Bs;

	if(H >= 0 && H < M_PI/3) {
		Rs = C;
		Gs = X;
		Bs = 0;	
	}
	else if(H >= M_PI/3 && H < 2*M_PI/3) {	
		Rs = X;
		Gs = C;
		Bs = 0;	
	}
	else if(H >= 2*M_PI/3 && H < M_PI) {
		Rs = 0;
		Gs = C;
		Bs = X;	
	}
	else if(H >= -M_PI && H < -2*M_PI/3) {
		Rs = 0;
		Gs = X;
		Bs = C;	
	}
	else if(H >= -2*M_PI/3 && H < -M_PI/3) {
		Rs = X;
		Gs = 0;
		Bs = C;	
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;	
	}
	/* printf("m: %Lf\n", m); */
	/* printf("C: %Lf\n", C); */
	/* printf("X: %Lf\n", X); */
	
	ret[0] = (Rs + m);
	ret[1] = (Gs + m);
	ret[2] = (Bs + m);
}

/*
void drawGraph(float* posData, cplx* operations, int opnum, GLfloat* colors, int height, int width, long double zoom) {
	cplx drawTemp;
	long double zoomc = zoom > 1.0 ? (1.0/4.0)*((1.0/2.0)+(1.0/log(zoom+(cpowl(M_E, 2.0/3.0)+1.0)-1.0))) : 0.5;
	printf("%Lf: %Lf\n", zoom, zoomc);
	//Speed this the hell up
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*3; j+=3) {
			//drawTemp = evalFunc(operations, opnum, posData[i*height*2+(j/3)*2] + posData[i*height*2+(j/3)*2+1] * I);
			drawTemp = evalFunc(operations, opnum, zoom * (posData[i*height*2+(j/3)*2] + posData[i*height*2+(j/3)*2+1] * I));
			//Have this scale with zoom
			hsv2rgb(cargl(drawTemp), 1.0 - cpowl(zoomc, cabsl(drawTemp)), 1, &(colors[i*height*3+j]) );
		}
	}
}
*/
