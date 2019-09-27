#version 410
layout (location = 0) in vec2 position;

uniform mat4 uProjection;
uniform vec3 uColor;
uniform vec4 uBorderColor;
out vec3 oColor;
out vec4 oBorderColor;

void main() {
    gl_Position = uProjection * vec4(position.xy, 0, 1);
    oColor = uColor;
    oBorderColor = uBorderColor;
}