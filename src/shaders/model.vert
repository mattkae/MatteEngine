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

uniform mat4 u_mvp;
uniform int u_numLights;
uniform vec4 u_fragPos;
uniform mat4 u_shadowMatrix[MAX_LIGHTS];

void main() {
  gl_Position = u_mvp * vec4(position, 1.0f);

  for (int index = 0; index < u_numLights; index++)
    o_ShadowCoords[index] = u_shadowMatrix[index] * vec4(position, 1.0f);

  o_FragPos = u_fragPos;
  o_TexCoords = texCoords;
  o_Normal = normal;
  o_Eye =  vec3(u_mvp[0][3], u_mvp[1][3], u_mvp[2][3]);
}
