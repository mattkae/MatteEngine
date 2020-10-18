#version 410

// Includes
#include Material.glsl
#include Light.shared.cpp
#include Light.glsl

// Input from vertex shader
in vec4 vFragPos;
in vec3 vViewDir;
in vec4 vClipSpaceCoordinates;
in vec2 vTexCoords;
in float vPhase;
in mat3 vTBN;

// Uniform variables
uniform int uNumLights;
uniform Light uLights[MAX_LIGHTS];
uniform Material uMaterial;
uniform vec2 uFarNear;
uniform sampler2DShadow uDirShadow[MAX_LIGHTS];
uniform sampler2D uReflection;
uniform sampler2D uRefraction;
uniform sampler2D uDudvMap;
uniform float uDudvMoveFactor;
uniform sampler2D uNormalMap;

// Out color
out vec4 Color;

const float distortionPower = 0.02;

void main() {
    vec4 finalColor = vec4(uMaterial.diffuse, uMaterial.opacity);
	vec2 distortionTexCoords = vTexCoords + vec2(uDudvMoveFactor, 0.0);
	vec3 normal = texture(uNormalMap, distortionTexCoords).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(vTBN * normal);

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
            uMaterial, normal, vViewDir, uMaterial.diffuse, uMaterial.specular, vFragPos);
        
        finalColor += vec4(lightColor.x, lightColor.y, lightColor.z, 0.0);
    }
	
	
	vec2 distortionOffset = distortionPower * ((texture(uDudvMap, distortionTexCoords).xy) * 2.0 - 1.0);

	vec2 textureCoordinate = clamp(distortionOffset + ((vClipSpaceCoordinates.xy / vClipSpaceCoordinates.w) / 2 + 0.5), 0.001, 0.999); // Clamp to account for the distortion moving us off of the texture

	float refractiveFactor = dot(vViewDir, normal);
	// @TODO: If you rais the refractiveFactor to a power, it becomes more reflective

	vec2 reflectionTexCoord = vec2(textureCoordinate.x, -textureCoordinate.y);
	vec2 refractionTexCoord = vec2(textureCoordinate.x, textureCoordinate.y);

	vec4 reflectColor = texture(uReflection, reflectionTexCoord);
	vec4 refractColor = texture(uRefraction, refractionTexCoord);
	Color = mix(reflectColor, refractColor, refractiveFactor);
    Color = mix(Color, finalColor, 0.2);
}