#version 410

layout (location = 0) in vec4 vertex;
out vec2 oTexCoords;

uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * vec4(vertex.xy, 0, 1);
    oTexCoords = vertex.zw;
}