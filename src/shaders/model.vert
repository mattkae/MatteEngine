#version 410 core

#define MAX_SHADOW_COORDS 4
#define MAX_SHADOW_MATRIX 4

// Layouts have the added benefit of being constant
// throughout shader programs.
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 o_FragPos; // Position of fragment in word space
out vec3 o_Normal;
out vec2 o_TexCoords;
out vec3 o_Eye;
out vec4 o_ShadowCoords[MAX_SHADOW_COORDS];

uniform mat4 u_mvp;
uniform vec3 u_eye;
uniform mat4 u_shadowMatrix[MAX_SHADOW_MATRIX];
uniform int u_numLights;
uniform vec4 u_fragPos;

void main() {
  gl_Position = mvp * vec4(position, 1.0f);;

  for (int index = 0; index < numLights; index++)
    o_ShadowCoords[index] = u_shadowMatrix * vec4(position, 1.0f);
  
  o_FragPos = u_fragPos;
  o_TexCoords = texCoords;
  o_Normal = normal;
  o_Eye = u_eye;
}
