#version 410

#include MaterialOut.glsl

flat in int vertexLayer;

#include primary.frag

uniform sampler2D uDiffuse[3];
uniform sampler2D uSpecular[3];
uniform sampler2D uNormal[3];

MaterialOut getMaterialOut() {
	MaterialOut result;

	result.ambient = vec3(0.5, 0.5, 0.5);
	result.diffuse = texture(uDiffuse[vertexLayer], vertexTexCoords).rgb;
	result.diffuseProperty = vec3(0.1, 0.1, 0.1);
	result.shininess = 96;
	result.opacity = 1;
	result.specular = texture(uSpecular[vertexLayer], vertexTexCoords).rgb;
	result.normal = texture(uNormal[vertexLayer], vertexTexCoords).rgb;
	result.normal = result.normal * 2.0 - 1.0;
	result.normal = normalize(vertexTBN * result.normal);

	return result;
}