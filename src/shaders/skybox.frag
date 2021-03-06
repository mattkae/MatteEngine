#version 410

smooth in vec3 oViewDir;

uniform samplerCube uSkyBox;

out vec4 Color;

void main() {
  Color = texture(uSkyBox, oViewDir);
}
