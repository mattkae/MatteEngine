#version 330 core

// Layouts have the adde benefit of being constant
// throughout shader programs.
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos; // Position of fragment in word space
out vec3 Normal;
out vec2 TexCoords;
out vec3 Eye;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye;

void main() {
  gl_Position = projection * view  * model * vec4(position, 1.0f);
  FragPos = vec3(model * vec4(position, 1.0f));
  TexCoords = texCoords;
  Normal = normal;
  Eye = eye;
}
