#include "main.h"

int main(void) {
	FILE* sample = fopen("input.txt", "r");
	/* FILE* sample = fopen("input3.txt", "r"); */
	char function[1024];
	fgets(function, 1024, sample);
	fclose(sample);

	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	int count = 0;
	queue out = queueInit();
	result res = parse_string(function, &out, &count);
	if( res.nerrs ) {
		exit(0);
	}
	cplx* output = (cplx*)malloc(count*sizeof(cplx));
	int cur = 0;

	while(front(out) != INT_MIN) {
		output[cur++] = dequeue(&out);
	}

	cplx val = evalFunc(output, count, 1);
	printf("Eval: %lf+%lfi\n", creal(val), cimag(val));

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Complex Function Grapher", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GLFWwindow* display = glfwCreateWindow(600, 600, "Display", NULL, NULL);
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
		printf("Error: compile error for shader vboVert\n");
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
		printf("Error: compile error for program shaderProgram\n");
	}
	glUseProgram(shaderProgram);

	GLuint vao;
	GLuint triangleVBO;
	float* posData;
	GLfloat* colors;
	int width = 1600;
	/* int width = 2000; */
	int height = 2000;
	double interval = 0.001;
	glPointSize(interval * 500);

	posData = malloc(sizeof(float) * 2 * width * height);
	colors = malloc(sizeof(GLfloat) * 3 * width * height);

	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*2; j+=2) {
			posData[i*height*2+j] = -1.0 + i*interval;
			posData[i*height*2+j+1] = -1.0 + (j/2)*interval;
		}
	}

	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*2; j+=2) {
			/* printf("%1.2f, %1.2f  ", posData[i*height+j], posData[i*height+j+1]); */
			/* printf("%1.3f,   ", posData[i*height*2+j]); */
			/* printf("%1.3f,   ", posData[i*height*2+j+1]); */
		}
		/* printf("\n"); */
	}

	for(int i = 0; i < width * 4; i++) {
		printf("(%d) %1.3f\n", i, posData[i]);
	}

	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*3; j+=3) {
			colors[i*height*3+j] = 0.1;
			colors[i*height*3+j+1] = 0.1;
			colors[i*height*3+j+2] = 0.1;
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * width * height + sizeof(GLfloat) * 3 * width * height, NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * width * height, posData);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 2 * width * height, sizeof(GLfloat) * 3 * width * height, colors);

	printf("GLfloat: %ld, float: %ld\n", sizeof(GLfloat), sizeof(float));

	//Position
	GLint posAttrb = glGetAttribLocation(shaderProgram, "position");

	glVertexAttribPointer(posAttrb, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(posAttrb);

	//Color
	printf("colors size: %ld\n", sizeof(GLfloat) * 3 * width * height);

	GLint colorAttrb = glGetAttribLocation(shaderProgram, "color");

	glVertexAttribPointer(colorAttrb, 3, GL_FLOAT, GL_FALSE, 0, sizeof(float) * 2 * width * height);

	glEnableVertexAttribArray(colorAttrb);

	/* glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); */

	while(!glfwWindowShouldClose(window) && !glfwWindowShouldClose(display)) {
		//input
		glfwPollEvents();

		//render
		/* glClearColor(0.2f, 0.68f, 0.08f, 1.0f); */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw VBO
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, width * height);
		
		//Events
		glfwSwapBuffers(window);
		glfwSwapBuffers(display);
	}

	glfwTerminate();

	// Exit on failure if there were errors.
	return 0;
}

char* filetobuf(char *file) {
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "rb"); /* Open file for reading */
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
