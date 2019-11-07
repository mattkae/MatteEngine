struct Light {
    vec3 direction;
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float cosineCutOff;
    float dropOff;
    bool usesShadows;
    mat4 shadowMatrix;
};