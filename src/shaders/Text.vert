#version 410

layout (location = 0) in vec2 texCoord;
layout (location = 1) in vec2 position;
layout (location = 2) in vec4 color;
layout (location = 3) in vec4 clip;

out vec2 oTexCoords;
out vec4 oColor;
out vec4 vertexClip;
out vec2 vertexPosition;

uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * vec4(position.x, position.y, 0, 1);
    oTexCoords = texCoord;
    oColor = color;
	vertexClip = clip;
	vertexPosition = position;
}