#version 430

layout (location = 0) in vec2 position;

uniform mat3 uView;
uniform mat4 uProjection;
smooth out vec3 oViewDir;

// See https://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330 for explanation.

void main() {
  vec3 unprojected = (uProjection * vec4(position, 1.0, 1.0)).xyz;
  oViewDir = uView * unprojected;

  gl_Position = vec4(position, 0.0, 1.0);
}
