#version 410

// Includes
#include Material.glsl
#include Light.shared.cpp
#include Light.glsl

// Input from vertex shader
in vec3 vNormal;
in vec4 vFragPos;
in vec3 vViewDir;

// Uniform variables
uniform int uNumLights;
uniform Light uLights[MAX_LIGHTS];
uniform Material uMaterial;
uniform vec2 uFarNear;
uniform sampler2DShadow uDirShadow[MAX_LIGHTS];

// Out color
out vec4 Color;

void main() {
    vec4 finalColor = vec4(uMaterial.diffuse, uMaterial.opacity);

    for (int lightIndex = 0; lightIndex < uNumLights; lightIndex++) {
        float visibility = 1.0;

        Light light = uLights[lightIndex];
        // @TODO: Not using shadows on the water yet, may want to do that in the future
        /*vec4 fragPosInLightSpace = light.shadowMatrix * vFragPos;
        if (light.usesShadows) {
            if (light.direction == vec3(0.0)) {
                visibility = getOmniVisibility(light, vFragPos, uFarNear);
            } else {
                visibility = getDirVisibility(lightIndex, fragPosInLightSpace, uDirShadow);
            }
        }*/

        vec3 lightColor = getColorFromLight(uLights[lightIndex], 
            uMaterial, vNormal, vViewDir, uMaterial.diffuse, uMaterial.specular, vFragPos);
        
        finalColor += vec4(lightColor.x, lightColor.y, lightColor.z, 0.0);
    }

    Color = finalColor;
}