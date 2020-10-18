#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

uniform mat4 uView;
uniform mat4 uProjection;

smooth out vec4 vColor;

void main() {
	 gl_Position = uProjection * uView * vec4(position, 1.0);
	 vColor = color;
}