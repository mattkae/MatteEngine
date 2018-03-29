#version 410 core;

layout (location = 0) in vec3 position;
layout (locaiton = 1) in vec3 color;;

uniform mat4 uVp;
uniform mat4 uModel;

void main() {
  gl_Position = uVp * uModel;
}
