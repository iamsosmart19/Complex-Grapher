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
	GLuint triangleVBO;	
	GLuint shaderProgram;
	GLchar *vboVertSource, *vboFragSource;
	GLuint vboVert, vboFrag;
	const unsigned int shaderAttribute = 0;

	float vboPoints[3][3] = {
		{0.0, 1.0, 0.0},
		{-1.0, -1.0, 0.0},
		{1.0,  -1.0, 0.0}
	};

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vboPoints), vboPoints, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderAttribute);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	vboVertSource = filetobuf("vertShad.vert");
	vboFragSource = filetobuf("fragShad.frag");

	vboVert = glCreateShader(GL_VERTEX_SHADER);
	vboFrag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vboVert, 1, (const GLchar**)&vboVertSource, 0);
	glShaderSource(vboFrag, 1, (const GLchar**)&vboFragSource, 0);

	free(vboVertSource);
	free(vboFragSource);

	glCompileShader(vboVert);
	glCompileShader(vboFrag);

	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vboVert);
	glAttachShader(shaderProgram, vboFrag);

	glBindAttribLocation(shaderProgram, shaderAttribute, "in_Position");
	glLinkProgram(shaderProgram);

	/* glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); */

	while(!glfwWindowShouldClose(window))
	{
		//input

		//render
		glUseProgram(shaderProgram);
		/* glClearColor(0.2f, 0.68f, 0.08f, 1.0f); */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, (sizeof(vboPoints) / 3) / (sizeof(GLfloat)));
		
		//Events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	// Exit on failure if there were errors.
	return !!res.nerrs;
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
