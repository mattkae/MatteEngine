#version 410
#include Light.shared.cpp
#define MAX_TEXTURES 3

#include Material.glsl
#include Light.glsl

// Output color
out vec4 Color;

// Input from vertex shader
in vec4 oFragPos;
in vec3 oNormal;
in vec3 oTexWeights;
in vec2 oTexCoords;
in vec3 vertexViewDir;
in mat3 vertexTBN;
in vec3 vertexDebugColor;

// Uniform variables
uniform Material uMaterial;
uniform sampler2D uDiffuseList[MAX_TEXTURES];
uniform sampler2D uSpecularList[MAX_TEXTURES];
uniform sampler2D uAmbientList[MAX_TEXTURES];
uniform sampler2D uNormalMapList[MAX_TEXTURES];

uniform int uNumLights;
uniform vec3 uAmbient;
uniform vec2 uFarNear;
uniform Light uLights[MAX_LIGHTS];
uniform sampler2DShadow uDirShadow[MAX_LIGHTS];

void main() {
    vec3 normal = oNormal;
    vec3 diffuse = uMaterial.useTexture[0] ? vec3(0, 0, 0) : uMaterial.diffuse;
    vec3 specular = uMaterial.useTexture[1] ? vec3(0, 0, 0) : uMaterial.specular;
    vec3 emissive = uMaterial.emissive;
    vec3 ambient = uAmbient;
	ambient = ambient + (uMaterial.useTexture[2] ? vec3(0, 0, 0) : uMaterial.ambient);

    // @TODO Only using a single texture for now, this stuff got weird
    for (int textureIndex = 0; textureIndex < MAX_TEXTURES; textureIndex++) {
        if (uMaterial.useTexture[0]) {
            diffuse += oTexWeights[textureIndex] * texture(uDiffuseList[textureIndex], oTexCoords).rgb;
        }
        if (uMaterial.useTexture[1]) {
            specular += oTexWeights[textureIndex] * texture(uSpecularList[textureIndex], oTexCoords).rgb;
        }
        if (uMaterial.useTexture[2]) {
            ambient += oTexWeights[textureIndex] * texture(uAmbientList[textureIndex], oTexCoords).rgb;
        }
        if (uMaterial.useTexture[3] && oTexWeights[textureIndex] > 0.75) {
            normal = texture(uNormalMapList[textureIndex], vec2(oTexCoords.x, oTexCoords.y)).rgb;
            normal = normal * 2.0 - 1.0;
            normal = normalize(vertexTBN * normal);
        }
    }

    vec3 finalColor = ambient * diffuse + emissive;
    for (int lightIndex = 0; lightIndex < uNumLights; lightIndex++) {
        float visibility = 1.0;

        Light light = uLights[lightIndex];
        vec4 fragPosInLightSpace = light.shadowMatrix * oFragPos;
        if (light.usesShadows) {
            if (light.direction == vec3(0.0)) {
                visibility = getOmniVisibility(light, oFragPos, uFarNear);
            } else {
                visibility = getDirVisibility(lightIndex, fragPosInLightSpace, uDirShadow);
            }
        }

        finalColor += visibility * getColorFromLight(uLights[lightIndex], 
            uMaterial, normal, vertexViewDir, diffuse, specular, oFragPos);
    }

    Color = vec4(finalColor, uMaterial.opacity);
    
    // @Info @NoCheckin: Uncomment this if you want to output the vertexDebugColor
    // Color = vec4(vertexDebugColor, uMaterial.opacity);
}