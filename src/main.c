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

	// tell GLFW to capture our mouse
    /* glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); */

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glViewport(0, 0, 800, 600);

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
	float* data;
	int width = 100;
	int height = 100;
	double interval = 0.01;
	data = malloc(sizeof(float) * 2 * width * height);
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*2; j+=2) {
			data[i*height*2+j] = -0.5 + (width-i)*interval;
			data[i*height*2+j+1] = -0.5 + (height-(j/2))*interval;
		}
	}
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < 10*2; j+=2) {
			/* printf("%1.2f, %1.2f  ", data[i*height+j], data[i*height+j+1]); */
			printf("%1.3f, ", data[i*height+j]);
			printf("%1.3f,   ", data[i*height+j+1]);
		}
		printf("\n");
	}
	/* float data[12][2] = { */
	/* 	{0.0, 0.0}, */
	/* 	{0.5, 0.0}, */
	/* 	{0.5, 0.5}, */

	/* 	{0.0, 0.0}, */
	/* 	{0.0, 0.5}, */
	/* 	{-0.5, 0.5}, */

	/* 	{0.0, 0.0}, */
	/* 	{-0.5, 0.0}, */
	/* 	{-0.5, -0.5}, */

	/* 	{0.0, 0.0}, */
	/* 	{0.0, -0.5}, */
	/* 	{0.5, -0.5} */
	/* }; */

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * width * height, data, GL_DYNAMIC_DRAW);

	printf("sizeof(float) * 2 * width * height: %ld\n", sizeof(float) * 2 * width * height);

	GLint posAttrb = glGetAttribLocation(shaderProgram, "position");

	glVertexAttribPointer(posAttrb, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(posAttrb);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(interval * 500);
	
	/* glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); */

	while(!glfwWindowShouldClose(window))
	{
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
