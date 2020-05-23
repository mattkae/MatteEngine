#version 410
#include Light.glsl
#include Material.glsl
#include Light.shared.cpp

#define MAX_TEXTURES 4

// Output color
out vec4 Color;

// Input from vertex shader
in vec4 oFragPos;
in vec3 oNormal;
in vec3 oTexCoords;
in vec3 oEye;

// Uniform variables
uniform Material uMaterial;
uniform sampler2D uDiffuseList[MAX_TEXTURES];
uniform sampler2D uSpecularList[MAX_TEXTURES];
uniform sampler2D uAmbientList[MAX_TEXTURES];

uniform int uNumLights;
uniform vec3 uAmbient;
uniform vec2 uFarNear;
uniform Light uLights[MAX_LIGHTS];
uniform sampler2DShadow uDirShadow[MAX_LIGHTS];

// Helper functions
vec3 getColorFromLight(Light directionalLight, vec3 normal, vec3 viewDir, vec3 diffuse, vec3 specular);
vec3 getDiffuse(vec3 normal, vec3 lightDir, vec3 color, vec3 diffuse);
vec3 getSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color, vec3 specular);
float getOmniVisibility(const in Light light);
float getDirVisibility(const in int lightIndex, vec4 fragPosInLightSpace);

void main() {
    vec3 viewDir = normalize(oEye - oFragPos.xyz);
    vec3 normal = normalize(oNormal);
    vec3 diffuse = uMaterial.diffuse;
    vec3 specular = uMaterial.specular;
    vec3 emissive = uMaterial.emissive;
    vec3 ambient = uAmbient;

    vec2 textureCoordinates = vec2(oTexCoords.x, oTexCoords.y);
    int textureIndex = int(oTexCoords.z);

    if (uMaterial.useTexture[0]) {
        diffuse = texture(uDiffuseList[textureIndex], textureCoordinates).rgb;
    }
    if (uMaterial.useTexture[1]) {
        specular = texture(uSpecularList[textureIndex], textureCoordinates).rgb;
    }
    if (uMaterial.useTexture[2]) {
        ambient = texture(uAmbientList[textureIndex], textureCoordinates).rgb;
    }

    vec3 finalColor = ambient * diffuse + emissive;
    for (int lightIndex = 0; lightIndex < uNumLights; lightIndex++) {
        float visibility = 1.0;

        Light light = uLights[lightIndex];
        vec4 fragPosInLightSpace = light.shadowMatrix * oFragPos;
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
    vec3 posToFrag = light.position - oFragPos.xyz;
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
    vec3 diffuseFactor = diffuse * uMaterial.diffuseProperty * max(0.f, dot(normal, lightDir));
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
    float retval = 0;
    int N = 4;
    int twoN = N * 2;

    vec3 lightPosition = (vec3(fragPosInLightSpace.x, 
        fragPosInLightSpace.y, 
        fragPosInLightSpace.z) / fragPosInLightSpace.w) * 0.5 + 0.5;

    if (true) {
        return texture(uDirShadow[lightIndex], lightPosition);
    }

    vec3 transformedVec = vec3(lightPosition);
    float divisor = 0;
    vec2 texelSize = 1.0 / textureSize(uDirShadow[lightIndex], 0);

    for (int x = -N; x <= N; x++) {
        for (int y = -N; y <= N; y++) {
            vec2 transform = vec2(x, y) * texelSize;
            transformedVec.x = lightPosition.x + transform.x;
            transformedVec.y = lightPosition.y + transform.y;
            divisor += 1;
            retval += texture(uDirShadow[lightIndex], transformedVec);
        }
    }
    return retval / divisor;
}