#version 410

#include MaterialOut.glsl
#include Material.glsl

uniform Material uMaterial;
uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uAmbientMap;
uniform sampler2D uNormalMap;

#include primary.frag

MaterialOut getMaterialOut() {
	MaterialOut result;
	result.normal = vertexNormal;
    result.diffuse = uMaterial.useTexture[0] ? texture(uDiffuse, vertexTexCoords).rgb : uMaterial.diffuse;
    result.specular = uMaterial.useTexture[1] ? texture(uSpecular, vertexTexCoords).rgb : uMaterial.specular;
    result.emissive = uMaterial.emissive;
    result.ambient = uAmbient + (uMaterial.useTexture[2] ? texture(uAmbientMap, vertexTexCoords).rgb : uMaterial.ambient);
	result.diffuseProperty = uMaterial.diffuseProperty;
	
	if (uMaterial.useTexture[3]) {
	   result.normal = texture(uNormalMap, vec2(vertexTexCoords.x, vertexTexCoords.y)).rgb;
	   result.normal = result.normal * 2.0 - 1.0;
	   result.normal = normalize(vertexTBN * result.normal); 
	}
	result.shininess = uMaterial.shininess;
	return result;
}