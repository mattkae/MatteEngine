#version 410

layout (location = 0) in vec4 vertex;
out vec2 oTexCoords;
out vec4 oColor;

uniform mat4 uProjection;
uniform float uScrollX;
uniform vec4 uColor;

void main() {
    gl_Position = uProjection * vec4(vertex.x - uScrollX, vertex.y, 0, 1);
    oTexCoords = vertex.zw;
    oColor = uColor;
}