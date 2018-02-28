#version 410

layout (location = 0) in vec3 position;

uniform mat4 uVp;
uniform mat4 uModel;
uniform vec3 uEye;
uniform vec3 uNormal;

out vec3 vPosition;
out vec3 vNormal;

void main() {
  vec4 fragPos = uModel * vec4(position, 1.0);
  gl_Position = uVp * fragPos;
  vNormal = uNormal;
  vPosition = fragPos.xyz;
}
