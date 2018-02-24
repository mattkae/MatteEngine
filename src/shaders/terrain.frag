#version 410

out vec4 Color;

in float oDistanceToCamera;

void main() {
  float val = 1 / pow(1.05, oDistanceToCamera);
  Color = vec4(val, val, val, val);
}
