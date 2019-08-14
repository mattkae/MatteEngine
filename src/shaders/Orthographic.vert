#version 410
layout (location = 0) in vec2 position;

uniform mat4 uProjection;
uniform vec3 uColor;
out vec3 oColor;

void main() {
    gl_Position = uProjection * vec4(position.xy, -0.1, 1);
    oColor = uColor;
}