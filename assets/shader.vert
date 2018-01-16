#version 330 core

// Layouts have the adde benefit of being constant
// throughout shader programs.
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 FragPos; // Position of fragment in word space
out vec3 Normal;
out vec2 TexCoords;
out vec3 Eye;
out vec4 ShadowCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye;

uniform mat4 shadowMatrix;

void main() {
  FragPos = model * vec4(position, 1.0f);
  gl_Position = projection * view  * FragPos;
  ShadowCoord = shadowMatrix * FragPos;
  TexCoords = texCoords;
  Normal = normal;
  Eye = eye;
}
