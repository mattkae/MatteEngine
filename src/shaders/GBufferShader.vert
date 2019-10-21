#version 410
#include Material.glsl

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 oFragPos;
out vec3 oNormal;
out vec3 oDiffuse;
out vec3 oSpecular;
out vec3 oEmissive;
out vec3 oMaterialInfo;

uniform mat4 uModel;
uniform mat4 uVp;
uniform Material uMaterial;

void main()
{
    vec4 fragPos = uModel * vec4(position, 1.0);
    oFragPos = fragPos; 
    oNormal = normal;
    oDiffuse = uMaterial.diffuse;
    oSpecular = uMaterial.specular;
    oEmissive = uMaterial.emissive;
    oMaterialInfo = vec3(uMaterial.shininess, uMaterial.opacity, 1.0);

    gl_Position = uVp * fragPos;
}