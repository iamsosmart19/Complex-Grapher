#version 150

in vec4 position;
in vec4 color;
out vec4 vert_color;

void main() {
	gl_Position = position;
	vert_color = color;
}
