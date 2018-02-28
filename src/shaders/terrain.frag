#version 410

out vec4 Color;

// TODO: Lighting for terrain
in vec3 gNormal;
in vec3 gPosition;
smooth in vec3 gEdgeDistance;

uniform float uLineWidth;
uniform vec4 uLineColor;

void main() {
  float d = min(gEdgeDistance.x, min(gEdgeDistance.y, gEdgeDistance.z));
  float mixVal = 1 - smoothstep(uLineWidth - 1, uLineWidth + 1, d);

  // TODO: Hack
  if (d <= 0)
    discard;
  
  Color = uLineColor * mixVal;  
}
