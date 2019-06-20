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

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 oFragPos;
in vec3 oNormal;

uniform Material uMaterial;

void main()
{    
    gPosition = oFragPos;
    gNormal = normalize(oNormal);
    gAlbedoSpec.rgb = uMaterial.diffuse;
    gAlbedoSpec.a = 1.0;
} 