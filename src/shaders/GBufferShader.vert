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

out vec3 oFragPos;
out vec3 oNormal;
out vec4 oAlbedoSpec;

uniform mat4 uModel;
uniform mat4 uVp;
uniform Material uMaterial;

void main()
{
    vec4 fragPos = uModel * vec4(position, 1.0);
    oFragPos = vec3(fragPos); 
    oNormal = normal;
    oAlbedoSpec.rgb = uMaterial.diffuse;
    oAlbedoSpec.a = 1.0;

    gl_Position = uVp * fragPos;
}