#version 410 core

#define MAX_SHADOW_COORDS 4
#define MAX_SHADOW_MATRIX 4

// Layouts have the added benefit of being constant
// throughout shader programs.
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 FragPos; // Position of fragment in word space
out vec3 Normal;
out vec2 TexCoords;
out vec3 Eye;
out vec4 ShadowCoords;//[MAX_SHADOW_COORDS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye;
uniform mat4 shadowMatrixProj;//[MAX_SHADOW_MATRIX];
uniform mat4 shadowMatrixView;
uniform int numLights;

void main() {
  vec4 worldFragPos = model * vec4(position, 1.0f);
  gl_Position = projection * view  * worldFragPos;

  ShadowCoords = shadowMatrixProj * shadowMatrixView * worldFragPos;
  //for (int index = 1; index < numLights; index++)
  //ShadowCoords[index] = shadowMatrix * worldFragPos;
  
  FragPos = worldFragPos;
  TexCoords = texCoords;
  Normal = normal;
  Eye = eye;
}
