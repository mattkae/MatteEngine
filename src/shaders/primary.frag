#include Light.shared.cpp

#include Light.glsl

// Output color
out vec4 Color;

// Input from vertex shader
in vec4 vertexFragPos;
in vec3 vertexNormal;
in vec2 vertexTexCoords;
in vec3 vertexViewDir;
in mat3 vertexTBN;

uniform int uNumLights;
uniform vec3 uAmbient;
uniform vec2 uFarNear;
uniform Light uLights[MAX_LIGHTS];
uniform sampler2DShadow uDirShadow[MAX_LIGHTS];

MaterialOut getMaterialOut();

void main() {
	MaterialOut outMat = getMaterialOut();

    vec3 finalColor = outMat.ambient * outMat.diffuse + outMat.emissive;
    for (int lightIndex = 0; lightIndex < uNumLights; lightIndex++) {
        float visibility = 1.0;

        Light light = uLights[lightIndex];
        vec4 fragPosInLightSpace = light.shadowMatrix * vertexFragPos;
        if (light.usesShadows) {
            if (light.direction == vec3(0.0)) {
                visibility = getOmniVisibility(light, vertexFragPos, uFarNear);
            } else {
                visibility = getDirVisibility(lightIndex, fragPosInLightSpace, uDirShadow);
            }
        }

        finalColor += visibility * getColorFromLight(uLights[lightIndex], 
           	outMat.normal, vertexViewDir, outMat.diffuse, outMat.diffuseProperty,
			outMat.specular, outMat.shininess, vertexFragPos);
    }

    Color = vec4(finalColor, outMat.opacity);
}