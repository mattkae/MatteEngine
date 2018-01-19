#version 410 core

layout(location = 0) out vec4 fragmentdepth;

void main() {
  fragmentdepth = vec4(0, 0, 0, 1.0);
}
