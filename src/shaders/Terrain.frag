#version 410

#include Light.shared.cpp
#include Light.glsl

in vec3 vertexNormal;
in vec4 vertexFragPos;
flat in int vertexLayer;
in vec2 vertexTexCoords;
in vec3 vertexViewDir;
in mat3 vertexTBN;

uniform sampler2D uDiffuse[3];
uniform sampler2D uSpecular[3];
uniform sampler2D uNormal[3];
uniform vec3 uAmbient;
uniform vec2 uFarNear;
uniform int uNumLights;
uniform Light uLights[MAX_LIGHTS];

out vec4 Color;

void main() {
	 vec3 diffuse = texture(uDiffuse[vertexLayer], vertexTexCoords).rgb;
	 vec3 specular = texture(uSpecular[vertexLayer], vertexTexCoords).rgb;
	 vec3 normal = texture(uNormal[vertexLayer], vertexTexCoords).rgb;
	 normal = normalize(vertexTBN * normal);

	 vec3 finalColor = vec3(0.0, 0.0, 0.0);
	 for (int lightIndex = 0; lightIndex < uNumLights; lightIndex++) {
	 	finalColor += getColorFromLight(uLights[lightIndex],
				   	normal, vertexViewDir,
					diffuse, vec3(0.1, 0.1, 0.1),
					specular, 96, vertexFragPos);
	 }

	 Color = vec4(finalColor, 1.0);
}