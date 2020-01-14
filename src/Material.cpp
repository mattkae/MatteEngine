#include "Material.h"
#include "TextureUniformConstants.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <string>

const int MATERIAL_TEX_INDEX = 8;

inline void renderTexture(const Shader &shader, GLint index, GLuint textureHandle, const GLchar* uniformName) {
 	setShaderInt(shader, uniformName, index);
	if (textureHandle > 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
	}
}

void render_material(const Shader &shader, const Material &material) {
	setShaderVec3(shader, "uMaterial.diffuse", material.diffuse);
	setShaderVec3(shader, "uMaterial.specular", material.specular);
	setShaderVec3(shader, "uMaterial.emissive", material.emissive);
	setShaderVec3(shader, "uMaterial.diffuseProperty", material.diffuseProperty);
	setShaderVec3(shader, "uMaterial.specularProperty", material.specularProperty);
	//setShaderFloat(shader, "uMaterial.shininess", material.specularComponent);
	setShaderFloat(shader, "uMaterial.opacity", material.transparency);

	setShaderBVec3(shader, "uMaterial.useTexture", material.diffuseTexture > 0, material.specularTexture > 0, material.ambientTexture > 0);
	renderTexture(shader, TextureUniformConstants::DIFFUSE_TEXTURE_POSTIION, material.diffuseTexture, "uMaterial.diffuseTexture");
	renderTexture(shader, TextureUniformConstants::SPECULAR_TEXTURE_POSTIION, material.specularTexture, "uMaterial.specularTexture");
	renderTexture(shader, TextureUniformConstants::AMBIENT_TEXTURE_POSITION, material.ambientTexture, "uMaterial.ambientTexture");
}
