#version 410

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    vec3 diffuseProperty;
    vec3 specularProperty;
    float shininess;
    float opacity;

    int texCount;
    sampler2D diffuseTex;
    sampler2D specularTex;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 oFragPos;
out vec3 oNormal;
out vec4 oAlbedoSpec;

uniform mat4 uModel;
uniform mat4 uVp;
uniform Material uMaterial;

void main()
{
    vec4 fragPos = uModel * vec4(position, 1.0);
    oFragPos = fragPos; 
    oNormal = normal;
    oAlbedoSpec = vec4(uMaterial.diffuse, uMaterial.shininess);

    gl_Position = uVp * fragPos;
}