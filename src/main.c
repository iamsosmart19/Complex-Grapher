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
	cplx* operations = (cplx*)malloc(count*sizeof(cplx));
	int cur = 0;

	while(front(out) != INT_MIN) {
		operations[cur++] = dequeue(&out);
	}

	/* cplx val = evalFunc(operations, count, 1); */
	/* printf("Eval: %llf+%llfi\n", creal(val), cimag(val)); */

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
	/* int width = 1600; */
	/* /1* int width = 2000; *1/ */
	/* int height = 2000; */
	/* double interval = 0.001; */
	int width = 800;
	/* int width = 2000; */
	int height = 1000;
	double interval = 0.002;
	glPointSize(interval * 500);

	posData = malloc(sizeof(float) * 2 * width * height);
	colors = malloc(sizeof(GLfloat) * 3 * width * height);

	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*2; j+=2) {
			posData[i*height*2+j] = -1.0 + i*interval;
			posData[i*height*2+j+1] = -1.0 + (j/2)*interval;
		}
	}

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

	/* glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); */

	int graphDrawn = 0;

	while(!glfwWindowShouldClose(window) && !glfwWindowShouldClose(display)) {
		//input
		glfwPollEvents();

		//render
		if(!graphDrawn) {
			drawGraph(posData, operations, count, colors, height, width);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * width * height, posData);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 2 * width * height, sizeof(GLfloat) * 3 * width * height, colors);
			graphDrawn = 1;
		}

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

void drawGraph(float* posData, cplx* operations, int opnum, GLfloat* colors, int height, int width) {
	cplx drawTemp;
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height*3; j+=3) {
			/* printf("imag: %lf%+lfi\n", posData[i*height*2+(j/3)*2], posData[i*height*2+(j/3)*2+1]); */
			/* printf("imag: %lf%+lf\n", creal(drawTemp), cimag(drawTemp)); */
			/* drawTemp = evalFunc(operations, opnum, posData[i*height*2+(j/3)*2] + posData[i*height*2+(j/3)*2+1] * I); */
			drawTemp = evalFunc(operations, opnum, 5 * (posData[i*height*2+(j/3)*2] + posData[i*height*2+(j/3)*2+1] * I));
			hsv2rgb(cargl(drawTemp), 1.0 - cpowl(0.5, cabsl(drawTemp)), 1, &(colors[i*height*3+j]) );
			/* hsv2rgb(cargl(drawTemp), 1.0 - cpowl(0.5, cabsl(drawTemp)), 1, ret ); */
			/* printf("%Lf %Lf %Lf\n%Lf, %Lf, %Lf\n\n", drawH, drawS, drawV, ret[0], ret[1], ret[2]); */

			/* colors[i*height*3+j] = ret[0]; */
			/* colors[i*height*3+j+1] = ret[1]; */
			/* colors[i*height*3+j+2] = ret[2]; */
		}
	}
}
