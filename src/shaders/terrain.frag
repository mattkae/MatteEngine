#version 410

out vec4 Color;

// TODO: Lighting for terrain
in vec3 gNormal;
in vec3 gPosition;
smooth in vec3 gEdgeDistance;
flat in vec3 gColor;

uniform float uLineWidth;
uniform int uShowWireframe;

void main() {
  float d = min(gEdgeDistance.x, min(gEdgeDistance.y, gEdgeDistance.z));

  float mixVal = 1;
  if (uShowWireframe == 0) {
    mixVal -= smoothstep(uLineWidth - 1, uLineWidth + 1, d);
  }

  // TODO: Hack
  if (d <= 0)
    discard;
  
  Color = vec4(gColor, 1.0) * mixVal;  
}
