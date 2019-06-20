#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

in vec3 uEye;

out vec2 oTexCoords;
out vec3 oEye;

void main()
{
  oEye = uEye;
  oTexCoords = aTexCoords;
  gl_Position = vec4(aPos, 1.0);
}