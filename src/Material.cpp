#include "Material.h"
#include "TextureUniformConstants.h"
#include "Logger.h"
#include "LoadModel.h"
#include "ModelLoader.h"
#include <iostream>
#include <sstream>
#include <string>

void Material::initialize(LoadMaterial& material, List<GeneratedTexture>* list) {
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
			const GeneratedTexture* texture = list->getValue(textureIndex);
			if (texture->uniqueId == material.diffuseUniqueTextureId) {
				textureList.add(TextureType::DIFFUSE, texture->texture);
			} else if (texture->uniqueId == material.ambientUniqueTextureId) {
				textureList.add(TextureType::AMBIENT, texture->texture);
			} else if (texture->uniqueId == material.specularUniqueTextureId) {
				textureList.add(TextureType::SPECULAR, texture->texture);
			}
		}
	}
}


void Material::render(const Shader& shader) const {
	setShaderVec3(shader, "uMaterial.diffuse", diffuse);
	setShaderVec3(shader, "uMaterial.specular", specular);
	setShaderVec3(shader, "uMaterial.emissive", emissive);
	setShaderVec3(shader, "uMaterial.diffuseProperty", diffuseProperty);
	setShaderVec3(shader, "uMaterial.specularProperty", specularProperty);
	//setShaderFloat(shader, "uMaterial.shininess", specularComponent);
	setShaderFloat(shader, "uMaterial.opacity", transparency);
	setShaderBVec3(shader, "uMaterial.useTexture", textureList.useTexture[0], textureList.useTexture[1], textureList.useTexture[2]);
	textureList.render(shader);
}
