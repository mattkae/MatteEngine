#version 410 core

out vec4 Color;

void main() {
  Color = vec4(gl_FragCoord.z, 0.0, 0.0, 1.0);
}
