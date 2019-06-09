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

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

uniform Material uMaterial;
uniform mat4 uModel;

void main()
{    
    gPosition = vec3(uModel * vec4(position, 1.0));
    gNormal = normalize(normal);
    gAlbedoSpec.rgb = uMaterial.diffuse;
    gAlbedoSpec.a = 1.0;
} 