#version 410
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

uniform mat4 uProjection;

out vec4 vertexColor;

void main() {
    gl_Position = uProjection * vec4(position.x, position.y, 0, 1);
    vertexColor = color;
}