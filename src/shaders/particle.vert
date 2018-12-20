#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 uVp;
uniform mat4 uModel;
uniform vec3 uPosition;

out vec3 vColor;

void main() {
  gl_Position = uVp * uModel * vec4(uPosition + position, 1.0);

  vColor = color;
}
