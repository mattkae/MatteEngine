#version 410

#define MAX_LIGHTS 2
#define MAX_DIR_LIGHT_SHADOWS 2

// Data types
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

// Output color
out vec4 Color;

// Input from vertex shader
in vec3 oFragPos;
in vec3 oNormal;
in vec2 oTexCoords;
in vec3 oEye;

// Uniform variables
uniform Material uMaterial;
uniform int uNumLights;
uniform Light uLights[MAX_LIGHTS];
uniform vec3 uAmbient;
uniform vec2 uFarNear;
uniform sampler2DShadow uDirShadow;
uniform mat4 uShadowMatrix;
// uniform samplerCubeShadow uOmniShadows[MAX_POINT_LIGHT_SHADOWS];

// Helper functions
vec3 getColorFromLight(Light directionalLight, vec3 normal, vec3 viewDir, vec3 diffuse, vec3 specular);
vec3 getDiffuse(vec3 normal, vec3 lightDir, vec3 color, vec3 diffuse);
vec3 getSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color, vec3 specular);
float getOmniVisibility(const in Light light);
float getDirVisibility(const in int lightIndex, vec4 fragPosInLightSpace);

void main() {
    vec3 viewDir = normalize(oEye - oFragPos);
    vec3 normal = normalize(oNormal);
    vec3 diffuse = uMaterial.diffuse;
    vec3 specular = uMaterial.specular;
    vec3 emissive = uMaterial.emissive;
    if (uMaterial.texCount > 0) {
        //diffuse = texture(uMaterial.diffuseTex, oTexCoords);
        //specular = texture(uMaterial.specularTex, oTexCoords);
    }

    vec4 fragPosInLightSpace = uShadowMatrix * vec4(oFragPos, 1);
    vec3 finalColor = uAmbient * diffuse + emissive;
    for (int lightIndex = 0; lightIndex < uNumLights; lightIndex++) {
        float visibility = 1.0;

        Light light = uLights[lightIndex];
        if (light.usesShadows) {
            if (light.direction == vec3(0.0)) {
                visibility = getOmniVisibility(light);
            } else {
                visibility = getDirVisibility(lightIndex, fragPosInLightSpace);
            }
        }

        finalColor += visibility * getColorFromLight(uLights[lightIndex], normal, viewDir, diffuse, specular);
    }

    Color = vec4(clamp(finalColor, vec3(0.0), vec3(1.0)), uMaterial.opacity);
}

vec3 getColorFromLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuse, vec3 specular) {
    // Get vector from the light, to the fragment
    vec3 posToFrag = light.position - oFragPos;
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
    vec3 diffuseFactor = diffuse * uMaterial.diffuseProperty *
                         max(0.f, dot(normal, lightDir));
    return color * diffuseFactor;
}

vec3 getSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color, vec3 specular) {
    vec3 reflection = reflect(-lightDir, normal);
    vec3 specularFactor = specular * uMaterial.specularProperty * pow(max(0.f, dot(viewDir, reflection)), 1);
    return color * specularFactor;
}

float getOmniVisibility(const in Light light) {
    vec3 lightToFrag = oFragPos.xyz - light.position;

    // Basicaly, we're moving our depth values into the
    // range of depth expected by the light's projection,
    // and then translating those values into device
    // space. See: https://en.wikipedia.org/wiki/Z-buffering#Mathematics
    vec3 absltof = abs(lightToFrag);
    float z = max(absltof.x, max(absltof.y, absltof.z));
    float depth = uFarNear.x + uFarNear.y / z;

    return 0.0; // texture(uOmniShadows[lightIndex], vec4(lightToFrag, depth));
}

float getDirVisibility(const in int lightIndex, vec4 fragPosInLightSpace) {
    vec3 lP = (fragPosInLightSpace.xyz / fragPosInLightSpace.w) * 0.5 + 0.5f;
    return texture(uDirShadow, lP);
}