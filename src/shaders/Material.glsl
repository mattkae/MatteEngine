struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    vec3 diffuseProperty;
    vec3 specularProperty;
    float shininess;
    float opacity;

    bvec3 useTexture;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D ambientTexture;
};