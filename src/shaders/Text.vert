#version 410

layout (location = 0) in vec4 vertex;
out vec2 oTexCoords;

uniform mat4 uProjection;
uniform int scrollX;

void main() {
    gl_Position = uProjection * vec4(vertex.x - scrollX, vertex.y, 0, 1);
    oTexCoords = vertex.zw;
}