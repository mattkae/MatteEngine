#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec3 vPosition[];

uniform mat4 uViewportMatrix;

out vec3 gNormal;
out vec3 gPosition;
smooth out vec3 gEdgeDistance;

// Drawn from the GLSL Shader Cookbook 4.0, but I understand it well,
// so it's cool.
void main() {
  // Move to viewport space (scren space);
  vec3 p0 = vec3(uViewportMatrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
  vec3 p1 = vec3(uViewportMatrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
  vec3 p2 = vec3(uViewportMatrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

  // Find distance to opposite edge (ha, hb, hc)
  float a = length(p1 - p2);
  float b = length(p2 - p0);
  float c = length(p1 - p0);
  float alpha = acos((b * b + c * c - a * a) / (2.0 * b * c));
  float beta = acos((a * a + c * c - b * b) / (2.0 * a * c));
  float ha = abs(c * sin(beta));
  float hb = abs(c * sin(alpha));
  float hc = abs(b * sin(alpha));

  gEdgeDistance = vec3(ha, 0, 0);
  gNormal = vNormal[0];
  gPosition = vPosition[0];
  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gEdgeDistance = vec3(0, hb, 0);
  gNormal = vNormal[1];
  gPosition = vPosition[1];
  gl_Position = gl_in[1].gl_Position;
  EmitVertex();
  
  gEdgeDistance = vec3(0, 0, hc);
  gNormal = vNormal[2];
  gPosition = vPosition[2];
  gl_Position = gl_in[2].gl_Position;
  EmitVertex();

  EndPrimitive();
}
