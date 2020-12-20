struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
	vec3 ambient;
    vec3 diffuseProperty;
    vec3 specularProperty;
    float shininess;
    float opacity;
    bvec4 useTexture;
};