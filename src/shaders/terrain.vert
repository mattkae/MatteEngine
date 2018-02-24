#version 410

layout (location = 0) in vec3 position;

uniform mat4 uVp;
uniform mat4 uModel;
uniform vec3 uEye;

out float oDistanceToCamera;

void main() {
  vec4 fragPos = uModel * vec4(position, 1.0);
  oDistanceToCamera = distance(uEye, fragPos.xyz);
  gl_Position = uVp * fragPos;
}
