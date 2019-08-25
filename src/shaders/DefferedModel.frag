#version 410

#define MAX_LIGHTS 2
#define MAX_DIR_LIGHT_SHADOWS 2

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
};

out vec4 FragColor;
  
in vec2 oTexCoords;

uniform sampler2D uPosition;
uniform sampler2D uNormal;
uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uEmissive;
uniform sampler2D uMaterialInfo;

uniform int uNumLights;
uniform Light uLights[MAX_LIGHTS];
uniform vec3 uAmbient;
uniform vec3 oEye;

vec3 getColorFromLight(Light directionalLight, vec3 fragPos, vec3 normal, vec3 viewDir, vec3 diffuse, vec3 specular);
vec3 getDiffuse(vec3 normal, vec3 lightDir, vec3 color, vec3 diffuse);
vec3 getSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color, vec3 specular);

void main()
{             
    vec3 fragPos = texture(uPosition, oTexCoords).rgb;
    vec3 normal = texture(uNormal, oTexCoords).rgb;
    vec3 diffuse = texture(uDiffuse, oTexCoords).rgb;
    vec3 specular = texture(uSpecular, oTexCoords).rgb;
    vec3 emissive = texture(uEmissive, oTexCoords).rgb;
    vec3 materialInfo = texture(uMaterialInfo, oTexCoords).rgb;
    vec3 viewDir = normalize(oEye - fragPos);
    
    vec3 lighting = emissive + diffuse * uAmbient;
    for(int i = 0; i < uNumLights; ++i)
    {
        lighting += getColorFromLight(uLights[i], fragPos, normal, viewDir, diffuse, specular);
    }
    
    FragColor = vec4(lighting, materialInfo.g);
}  


vec3 getColorFromLight(Light light, vec3 fragPos, vec3 normal, vec3 viewDir, vec3 diffuse, vec3 specular) {
    // Get vector from the light, to the fragment
    vec3 posToFrag = light.position - fragPos;
    float delta = length(posToFrag);
    posToFrag = normalize(posToFrag);

    // Point lights will have no light direction
    vec3 direction = (light.direction == vec3(0.0)) ? posToFrag : -light.direction;
    direction = normalize(direction);
    float normalDotDir = max(0.f, dot(normal, direction));
    if (normalDotDir == 0) {
        return vec3(0.f);
    }

    // Check if frag is within spot
    // Point and Direction have cutoff = 0.0 (so never less)
    float angleBetween = min(dot(posToFrag, direction), 1.0);
    if (angleBetween < light.cosineCutOff) {
        return vec3(0.f);
    }

    // Calculate intensity of the light (Directional and Point have no drop off)
    float attenuation = (light.constant + delta * light.linear +
                         delta * delta * light.quadratic);
    vec3 intensity =
        (light.color * pow(angleBetween, light.dropOff)) / attenuation;

    // Calculate diffuse and specular
    vec3 diffuseOut = getDiffuse(normal, direction, intensity, diffuse);
    vec3 specularOut = getSpecular(normal, direction, viewDir, intensity, specular);

    return diffuseOut + specularOut;
}

vec3 getDiffuse(vec3 normal, vec3 lightDir, vec3 color, vec3 diffuse) {
    vec3 diffuseFactor = diffuse * 1.0 * max(0.f, dot(normal, lightDir));
    return color * diffuseFactor;
}

vec3 getSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color, vec3 specular) {
    vec3 reflection = reflect(-lightDir, normal);
    vec3 specularFactor = specular * 1.0 * pow(max(0.f, dot(viewDir, reflection)), 1);
    return color * specularFactor;
}