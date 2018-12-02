#version 410 core

#define MAX_LIGHTS 2

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 oFragPos;
out vec3 oNormal;
out vec2 oTexCoords;
out vec3 oEye;
out vec4 oShadowCoords[MAX_LIGHTS];

uniform int uNumLights;
uniform mat4 uVp;
uniform mat4 uShadowMatrix[MAX_LIGHTS];
uniform mat4 uModel;
uniform vec3 uEye;

void main() {
  vec4 fragPos = uModel * vec4(position, 1.0f);
  gl_Position = uVp * fragPos;

  for (int index = 0; index < uNumLights; index++) {
     oShadowCoords[index] = uShadowMatrix[index] * vec4(position, 1.0f);
  }

  oFragPos = fragPos;
  oTexCoords = texCoords;
  oNormal = normal;
  oEye = uEye;
}
