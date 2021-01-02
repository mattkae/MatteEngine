#version 410
#include Light.shared.cpp

#include Material.glsl
#include Light.glsl

// Output color
out vec4 Color;

// Input from vertex shader
in vec4 oFragPos;
in vec3 oNormal;
in vec2 oTexCoords;
in vec3 vertexViewDir;
in mat3 vertexTBN;

// Uniform variables
uniform Material uMaterial;
uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uAmbientMap;
uniform sampler2D uNormalMap;

uniform int uNumLights;
uniform vec3 uAmbient;
uniform vec2 uFarNear;
uniform Light uLights[MAX_LIGHTS];
uniform sampler2DShadow uDirShadow[MAX_LIGHTS];

void main() {
    vec3 normal = oNormal;
    vec3 diffuse = uMaterial.useTexture[0] ? texture(uDiffuse, oTexCoords).rgb : uMaterial.diffuse;
    vec3 specular = uMaterial.useTexture[1] ? texture(uSpecular, oTexCoords).rgb : uMaterial.specular;
    vec3 emissive = uMaterial.emissive;
    vec3 ambient = uAmbient + (uMaterial.useTexture[2] ? texture(uAmbientMap, oTexCoords).rgb : uMaterial.ambient);
	
	if (uMaterial.useTexture[3]) {
	   normal = texture(uNormalMap, vec2(oTexCoords.x, oTexCoords.y)).rgb;
	   normal = normal * 2.0 - 1.0;
	   normal = normalize(vertexTBN * normal); 
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
           	normal, vertexViewDir, diffuse, uMaterial.diffuseProperty,
			specular, uMaterial.shininess, oFragPos);
    }

    Color = vec4(finalColor, uMaterial.opacity);
}