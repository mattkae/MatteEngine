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