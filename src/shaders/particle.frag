#version 430 core

in vec3 vColor;

out vec4 Color;
uniform float uTransparency;

void main() {
  Color = vec4(vColor, uTransparency);
}
