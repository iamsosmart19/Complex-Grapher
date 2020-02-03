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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glViewport(0, 0, 800, 600);

	/* glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); */

	while(!glfwWindowShouldClose(window))
	{
		//input

		//render
		glClearColor(0.2f, 0.68f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	// Exit on failure if there were errors.
	return !!res.nerrs;
}

