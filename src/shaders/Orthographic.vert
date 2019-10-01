#version 410
layout (location = 0) in vec2 position;

uniform mat4 uProjection;
uniform vec4 uColor;
out vec4 oColor;

void main() {
    gl_Position = uProjection * vec4(position.x, position.y, 0, 1);
    oColor = uColor;
}