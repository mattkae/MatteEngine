#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

const int MAX_LIGHTS = 4;

out vec4 o_FragPos;
out vec3 o_Normal;
out vec2 o_TexCoords;
out vec3 o_Eye;
out vec4 o_ShadowCoords[MAX_LIGHTS];

uniform int u_numLights;
uniform mat4 u_vp;
uniform mat4 u_shadowMatrix[MAX_LIGHTS];
uniform mat4 u_model;
uniform vec3 u_eye;

void main() {
  vec4 fragPos = u_model * vec4(position, 1.0f);
  gl_Position = u_vp * fragPos;

  for (int index = 0; index < u_numLights; index++) {
     o_ShadowCoords[index] = u_shadowMatrix[index] * fragPos;
  }

  o_FragPos = fragPos;
  o_TexCoords = texCoords;
  o_Normal = normal;
  o_Eye = u_eye;
}
