#include "Material.h"
#include "TextureUniformConstants.h"
#include "Logger.h"
#include "LoadModel.h"
#include "ModelLoader.h"
#include <iostream>
#include <sstream>
#include <string>

void Material::initialize(LoadMaterial& material, List<TextureListItem>* list) {
	emissive = material.emissive;
	ambient = material.ambient;
	diffuse = material.diffuse;
	specular = material.specular;
	diffuseProperty = material.diffuseProperty;
	specularProperty = material.specularProperty;
	specularComponent = material.specularComponent;
	transparency = material.transparency;

	if (list != nullptr) {
		for (size_t textureIndex = 0; textureIndex < list->numElements; textureIndex++) {
			const TextureListItem* texture = list->getValue(textureIndex);
			if (texture->uniqueId == material.diffuseUniqueTextureId) {
				diffuseTexture = texture->texture;
			} else if (texture->uniqueId == material.ambientUniqueTextureId) {
				ambientTexture = texture->texture;
			} else if (texture->uniqueId == material.specularUniqueTextureId) {
				specularTexture = texture->texture;
			}
		}
	}
}

inline void renderTexture(const Shader &shader, GLint index, GLuint textureHandle, const GLchar* uniformName) {
 	setShaderInt(shader, uniformName, index);
	if (textureHandle > 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
	}
}

void Material::render(const Shader &shader) const {
	setShaderVec3(shader, "uMaterial.diffuse", diffuse);
	setShaderVec3(shader, "uMaterial.specular", specular);
	setShaderVec3(shader, "uMaterial.emissive", emissive);
	setShaderVec3(shader, "uMaterial.diffuseProperty", diffuseProperty);
	setShaderVec3(shader, "uMaterial.specularProperty", specularProperty);
	//setShaderFloat(shader, "uMaterial.shininess", specularComponent);
	setShaderFloat(shader, "uMaterial.opacity", transparency);

	setShaderBVec3(shader, "uMaterial.useTexture", diffuseTexture > 0, specularTexture > 0, ambientTexture > 0);
	renderTexture(shader, TextureUniformConstants::DIFFUSE_TEXTURE_POSTIION, diffuseTexture, "uDiffuseList[0]");
	renderTexture(shader, TextureUniformConstants::SPECULAR_TEXTURE_POSTIION, specularTexture, "uSpecularList[0]");
	renderTexture(shader, TextureUniformConstants::AMBIENT_TEXTURE_POSITION, ambientTexture, "uAmbientList[0]");
}
