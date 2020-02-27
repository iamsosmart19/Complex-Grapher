#include "main.h"

int main(void) {
#ifdef test
	setenv("CUDA_CACHE_DISABLE", "1", 1);
#endif

	FILE* sample = fopen("input.txt", "r");
	/* FILE* sample = fopen("input3.txt", "r"); */
	char function[1024];
	fgets(function, 1024, sample);
	fclose(sample);

	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	queue out = queueInit();
	result res = parse_string(function, &out);
	if( res.nerrs ) {
		exit(0);
	}
	cplx* operations = (cplx*)malloc(128*sizeof(cplx));

	int count;
	while(front(out) != -DBL_MAX-DBL_MAX*I) {
		operations[count++] = dequeue(&out);
	}
	operations = (cplx*)realloc(operations, count*sizeof(cplx));
	printf("count: %d\n", count);

	//SDL
	TTF_Init();
	//The window we'll be rendering to
    SDL_Window* SDLwindow = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 1;
	}

	SDLwindow = SDL_CreateWindow( "SDL Window", 30, 200, 800, 600, SDL_WINDOW_SHOWN );

	screenSurface = SDL_GetWindowSurface( SDLwindow );

	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x12, 0x67, 0xBF ) );

	SDL_UpdateWindowSurface( SDLwindow );

	//renderer
	SDL_Renderer* SDLMainWindowRenderer = SDL_CreateRenderer(SDLwindow, -1, 0);

	//quit code
	int SDL_close_window = 0;
	SDL_Event shouldCloseWindow;

	//Text code;
	char textBoxString[2048] = "Enter a function";

	TTF_Font* gFont = TTF_OpenFont( "../comp-mod/cmuntt.ttf", 28 );

	if(gFont == NULL) {
		printf("font no exist\n");
	}

	SDL_Color textBoxColour = { 255, 255, 255, 255 };

	SDL_Surface* textBoxSurface = TTF_RenderText_Solid( gFont, textBoxString, textBoxColour);

	if(textBoxSurface == NULL) {
		printf("Surface render error\n");
	}

	SDL_Texture* textBoxTexture = SDL_CreateTextureFromSurface(SDLMainWindowRenderer, textBoxSurface);
	if(textBoxTexture == NULL) {
		printf("Texture render error: %s\n", SDL_GetError());
	}

	/* SDL_StartTextInput(); */

	//GLFW OpenGL code
	
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* display = glfwCreateWindow(600, 600, "Display", NULL, NULL);
	glfwSetWindowPos(display, 900, 200);
	glfwMakeContextCurrent(display);

	// tell GLFW to capture our mouse
    /* glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); */

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//Points
	glEnable(GL_PROGRAM_POINT_SIZE);

	//VBO
	GLuint shaderProgram;
	GLuint vboVert, vboFrag;
	char *vertSource, *fragSource;

	vertSource = filetobuf("vertShad.vert");
	vboVert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vboVert, 1, (const GLchar**)&vertSource, 0);
	glCompileShader(vboVert);

	GLint vertSuccess;
	glGetShaderiv(vboVert, GL_COMPILE_STATUS, &vertSuccess);
	if(vertSuccess != GL_TRUE) {
		printf("Error: compile error for shader vboVert: \n");
	}

	fragSource = filetobuf("fragShad.frag");
	vboFrag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vboFrag, 1, (const GLchar**)&fragSource, 0);
	glCompileShader(vboFrag);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vboVert);
	glAttachShader(shaderProgram, vboFrag);

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &vertSuccess);
	if(vertSuccess != GL_TRUE) {
		printf("Error: compile error for program shaderProgram: \n");
	}
	glUseProgram(shaderProgram);

	GLuint vao;
	GLuint triangleVBO;
	float* posData;
	float posOffset[2] = {0, 0};
	GLfloat* colors;
	/* int width = 1600; */
	/* int height = 2000; */
	/* double interval = 0.001; */
	int width = 800;
	int height = 1000;
	int n = width * height;
	double interval = 0.002;
	double zoom = 10;
	/* float zoomc = zoom > 1.0 ? (1.0/4.0)*((1.0/2.0)+(1.0/log(zoom+(cpowl(M_E, 2.0/3.0)+1.0)-1.0))) : 0.5; */
	float zoomc = 0.001;
	printf("zoomc: %f\n", zoomc);
	glPointSize(interval * 500);

    size_t posSize = sizeof(float) * 2 * n;
    size_t colorSize = sizeof(GLfloat) * 3 * n;

	posData = malloc(posSize);
	colors = malloc(colorSize);

	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*2; j+=2) {
			posData[i*height*2+j] = -1.0 + i*interval;
			posData[i*height*2+j+1] = -1.0 + (j/2)*interval;
		}
	}

	//OpenCL code;

    // Device input buffers
    cl_mem posBuffer;
	cl_mem opBuffer;
    // Device output buffer
    cl_mem colorBuffer;
 
    cl_platform_id cpPlatform;        // OpenCL platform
    cl_device_id device_id;           // device ID
    cl_context context;               // context
    cl_command_queue queue;           // command queue
    cl_program program;               // program
    cl_kernel kernel;                 // kernel
 
    size_t globalSize, localSize;
    cl_int err;
 
    // Number of work items in each local work group
    localSize = 512;
 
    // Number of total work items - localSize must be divisor
    globalSize = ceil(n/(float)localSize)*localSize;
 
    // Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);
 
	if(err != CL_SUCCESS) {
		printf("error: %d\n", err);
	}

    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if(err != CL_SUCCESS) {
		printf("error: %d\n", err);
	}

    // Create a context  
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if(err != CL_SUCCESS) {
		printf("error: %d\n", err);
	}
 
    // Create a command queue 
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);

	/* printf("BR: 0\n"); */

    // Create the compute program from the source buffer
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
	/* printf("|%s|", kernelSource); */
	/* printf("\n\n"); */


	const char* kSrcPtr = kernelSource;
    program = clCreateProgramWithSource(context, 1, (const char **)&kSrcPtr, NULL, &err);

	if(err != CL_SUCCESS) {
		printf("program build failed\n");
		printf("error: %d\n", err);
	}
 
    // Build the program executable 
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if(err != CL_SUCCESS) {
		printf("build error: %d\n", err);
		size_t errorSize;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &errorSize);
		
		char* log = malloc(errorSize);
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, errorSize, log, NULL);

		printf("|%s|\n", log);
	}

    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "graph", &err);
	if(err != CL_SUCCESS) {
		printf("link error: %d\n", err);
	}

	printf("count*sizeof(cplx): %ld\n", count * sizeof(cplx));
 
    // Create the input and output arrays in device memory for our calculation
    posBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, posSize, NULL, NULL);
	opBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, count*sizeof(cplx), NULL, NULL);

    colorBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, colorSize, NULL, NULL);
 
    // Write our data set into the input array in device memory
    err = clEnqueueWriteBuffer(queue, posBuffer, CL_TRUE, 0, posSize, posData, 0, NULL, NULL);
    err |= clEnqueueWriteBuffer(queue, opBuffer, CL_TRUE, 0, count*sizeof(cplx), operations, 0, NULL, NULL);

    // Set the arguments to our compute kernel
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &posBuffer);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &colorBuffer);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &opBuffer);
    err |= clSetKernelArg(kernel, 3, sizeof(int), &count);
    err |= clSetKernelArg(kernel, 4, sizeof(double), &zoom);
    err |= clSetKernelArg(kernel, 5, sizeof(float), &zoomc);
    err |= clSetKernelArg(kernel, 6, sizeof(float)*2, &posOffset);
    err |= clSetKernelArg(kernel, 7, sizeof(unsigned int), &n);
 
    // Execute the kernel over the entire range of the data set  
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
 
    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);
 
    // Read the results from the device
    clEnqueueReadBuffer(queue, colorBuffer, CL_TRUE, 0, colorSize, colors, 0, NULL, NULL);

#ifdef test
	cplx ret = -1.23 + 2.01*I;
	cplx ctestTemp;
	for(int i = 0; i < 26 * 3; i += 3) {
		switch(i/3) {
			case 0:
				ctestTemp = ret + ret;
				break;

			case 1:
				ctestTemp = ret - ret;
				break;

			case 2:
				ctestTemp = ret * ret;
				break;

			case 3:
				ctestTemp = ret / (12.3 - 3 * I);
				break;

			case 4:
				ctestTemp = creal(ret);
				break;

			case 5:
				ctestTemp = cimag(ret);
				break;

			case 6:
				ctestTemp = conj(ret);
				break;

			case 7:
				ctestTemp = carg(ret);
				break;

			case 8:
				ctestTemp = cproj(ret);
				break;

			case 9:
				ctestTemp = cabs(ret);
				break;

			case 10:
				ctestTemp = cexp(ret);
				break;

			case 11:
				ctestTemp = clog(ret);
				break;

			case 12:
				ctestTemp = cpow(ret, ret);
				break;

			case 13:
				ctestTemp = csqrt(ret);
				break;

			case 14:
				ctestTemp = csin(ret);
				break;

			case 15:
				ctestTemp = ccos(ret);
				break;

			case 16:
				ctestTemp = ctan(ret);
				break;

			case 17:
				ctestTemp = csinh(ret);
				break;

			case 18:
				ctestTemp = ccosh(ret);
				break;

			case 19:
				ctestTemp = ctanh(ret);
				break;

			case 20:
				ctestTemp = casinh(ret);
				break;

			case 21:
				ctestTemp = cacosh(ret);
				break;

			case 22:
				ctestTemp = catanh(ret);
				break;

			case 23:
				ctestTemp = casin(ret);
				break;

			case 24:
				ctestTemp = cacos(ret);
				break;

			case 25:
				ctestTemp = catan(ret);
					break;
		}

		printf("%d SHADER: %f%+fi\n", i/3, colors[i] , colors[i+1] );
		printf("%d COMPLX: %lf%+lfi\n", i/3, creal(ctestTemp) , cimag(ctestTemp));
		printf("%d: %f%+fi\n\n", i/3, colors[i] - creal(ctestTemp) , colors[i+1] - cimag(ctestTemp));
	}
#else
	printf("%f\n", posData[0]);
	printf("%f\n", posData[1]);
	printf("%f\n", colors[0]);
	printf("%f\n", colors[1]);
	printf("%f\n", colors[2]);
	printf("%f\n", colors[3]);
	printf("%f\n", colors[4]);
	printf("%f\n", colors[5]);
	printf("%f\n", colors[3*n-3]);
	printf("%f\n", colors[3*n-2]);
	printf("%f\n", colors[3*n-1]);
#endif

	/* for(int i = 0; i < width; i++) { */
	/* 	for(int j = 0; j < height*2; j+=2) { */
			/* printf("%1.2f, %1.2f  ", posData[i*height+j], posData[i*height+j+1]); */
			/* printf("%1.3f,   ", posData[i*height*2+j]); */
			/* printf("%1.3f,   ", posData[i*height*2+j+1]); */
		/* } */
		/* printf("\n"); */
	/* } */

	/* for(int i = 0; i < width * 4; i++) { */
	/* 	printf("(%d) %1.3f\n", i, posData[i]); */
	/* } */

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * width * height + sizeof(GLfloat) * 3 * width * height, NULL, GL_DYNAMIC_DRAW);

	/* printf("GLfloat: %ld, float: %ld\n", sizeof(GLfloat), sizeof(float)); */

	//Position
	GLint posAttrb = glGetAttribLocation(shaderProgram, "position");

	glVertexAttribPointer(posAttrb, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(posAttrb);

	//Color
	printf("colors size: %ld\n", sizeof(GLfloat) * 3 * width * height);

	GLint colorAttrb = glGetAttribLocation(shaderProgram, "color");

	glVertexAttribPointer(colorAttrb, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 2 * width * height));

	glEnableVertexAttribArray(colorAttrb);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * width * height, posData);

	/* glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); */

	glOrtho(-300, 300, -300, 300, 0, 0);

	int graphDrawn = 0;

	while(!glfwWindowShouldClose(display) && !SDL_close_window) {
		//input
		SDL_PollEvent(&shouldCloseWindow);
		if(shouldCloseWindow.type == SDL_QUIT) {
			SDL_close_window = 1;
		}

		glfwPollEvents();
		if (glfwGetKey(display, GLFW_KEY_COMMA) == GLFW_PRESS ){
			if (glfwGetKey(display, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ) {
				zoom += zoom/5;
			}
			else {
				zoom += zoom/20;
			}
			zoomc = zoom < 1.0 ? powl(0.001, 2.0-zoom) : 0.001;
			/* printf("zoomc: %.10f\n", zoomc); */
			graphDrawn = 0;
		}
		if (glfwGetKey(display, GLFW_KEY_PERIOD) == GLFW_PRESS ){
			if (glfwGetKey(display, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ) {
				zoom -= zoom/5;
			}
			else {
				zoom -= zoom/20;
			}
			zoomc = zoom < 1.0 ? powl(0.001, 2.0-zoom) : 0.001;
			/* printf("zoomc: %.10f\n", zoomc); */
			graphDrawn = 0;
		}

		//Movement code
		if (glfwGetKey(display, GLFW_KEY_UP) == GLFW_PRESS ) {
			if (glfwGetKey(display, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ) {
				posOffset[1] += fabs(log(zoom)) / 10;
			}
			else {
				posOffset[1] += fabs(log(zoom)) / 100;
			}
			graphDrawn = 0;
		}

		if (glfwGetKey(display, GLFW_KEY_DOWN) == GLFW_PRESS ) {
			if (glfwGetKey(display, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ) {
				posOffset[1] -= fabs(log(zoom)) / 10;
			}
			else {
				posOffset[1] -= fabs(log(zoom)) / 100;
			}
			graphDrawn = 0;
		}

		if (glfwGetKey(display, GLFW_KEY_LEFT) == GLFW_PRESS ) {
			if (glfwGetKey(display, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ) {
				posOffset[0] -= fabs(log(zoom)) / 10;
			}
			else {
				posOffset[0] -= fabs(log(zoom)) / 100;
			}
			graphDrawn = 0;
		}

		if (glfwGetKey(display, GLFW_KEY_RIGHT) == GLFW_PRESS ) {
			if (glfwGetKey(display, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ) {
				posOffset[0] += fabs(log(zoom)) / 10;
			}
			else {
				posOffset[0] += fabs(log(zoom)) / 100;
			}
			graphDrawn = 0;
		}

		if (glfwGetKey(display, GLFW_KEY_SPACE) == GLFW_PRESS ) {
			posOffset[0] = 0;
			posOffset[1] = 0;
			zoom = 10;
			graphDrawn = 0;
		}

		//render
		if(!graphDrawn) {
			err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &posBuffer);
			err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &colorBuffer);
			err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &opBuffer);
			err |= clSetKernelArg(kernel, 3, sizeof(int), &count);
			err |= clSetKernelArg(kernel, 4, sizeof(double), &zoom);
			err |= clSetKernelArg(kernel, 5, sizeof(float), &zoomc);
			err |= clSetKernelArg(kernel, 6, sizeof(float)*2, &posOffset);
			err |= clSetKernelArg(kernel, 7, sizeof(unsigned int), &n);
		 
			// Execute the kernel over the entire range of the data set  
			err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
		 
			// Wait for the command queue to get serviced before reading back results
			clFinish(queue);
		 
			// Read the results from the device
			clEnqueueReadBuffer(queue, colorBuffer, CL_TRUE, 0, colorSize, colors, 0, NULL, NULL);
			/* drawGraph(posData, operations, count, colors, height, width, zoom); */
			glBufferSubData(GL_ARRAY_BUFFER, posSize, colorSize, colors);
			graphDrawn = 1;
		}

		/* glClearColor(0.2f, 0.68f, 0.08f, 1.0f); */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* SDL_SetRenderDrawColor( SDLMainWindowRenderer, 0x12, 0x67, 0xBF, 0xFF ); */
		/* SDL_RenderClear( SDLMainWindowRenderer); */

		//draw VBO
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, width * height);

		//Draw Textbox
		SDL_RenderCopy(SDLMainWindowRenderer, textBoxTexture, NULL, NULL);
		SDL_RenderPresent(SDLMainWindowRenderer);
		
		//Events
		SDL_UpdateWindowSurface(SDLwindow);
		SDL_RenderPresent(SDLMainWindowRenderer);
		glfwSwapBuffers(display);

		printf("");
	}

	glfwTerminate();

    // release OpenCL resources
    clReleaseMemObject(posBuffer);
	clReleaseMemObject(opBuffer);
    clReleaseMemObject(colorBuffer);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

	//release SDL resources
	SDL_DestroyWindow( SDLwindow );
	TTF_CloseFont( gFont );
	SDL_DestroyTexture(textBoxTexture);
	/* SDL_FreeSurface(screenSurface); */

	TTF_Quit();
	SDL_Quit();

	// Exit on failure if there were errors.
	return 0;
}

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
	long double Hs = ((H * 180)/M_PI) > 0 ?  (H*180)/M_PI: 360 + (H*180)/M_PI;
	long double X = C * (1 - fabsl(fmodl(Hs/ 60.0, 2) - 1));
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
