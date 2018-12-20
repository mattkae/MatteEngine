#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 uVp;
uniform vec3 uEye;
uniform vec3 uNormal;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vColor;

void main() {
  vec4 fragPos = vec4(position, 1.0);
  gl_Position = uVp * fragPos;
  vNormal = uNormal;
  vPosition = fragPos.xyz;
  vColor = color;
}
