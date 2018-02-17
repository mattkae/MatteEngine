#version 410 core

layout (location = 0) in vec3 position;

uniform mat4 uViewProj;
uniform mat4 uModel;

void main() {
  gl_Position = uViewProj * uModel * vec4(position, 1.0f);
}
