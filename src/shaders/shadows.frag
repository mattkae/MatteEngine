#version 410

layout(location = 0) out vec4 fragmentdepth;

void main() {
  fragmentdepth = vec4(0, 0, 0, 1.0);
}
