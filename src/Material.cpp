#include "Material.h"
#include "Logger.h"
#include "LoadModel.h"
#include "ModelLoader.h"
#include "ShaderUniformMapping.h"

void Material::initialize(LoadMaterial& material, List<GeneratedTexture>* list) {
	emissive = material.emissive;
	ambient = material.ambient;
	diffuse = material.diffuse;
	specular = material.specular;
	diffuseProperty = material.diffuseProperty;
	specularProperty = material.specularProperty;
	specularComponent = material.specularComponent;
	transparency = material.transparency;
	shininess = material.shininess;

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


void Material::render(const MaterialUniformMapping& mapping) const {
	setShaderVec3(mapping.MATERIAL_DIFFUSE, diffuse);
	setShaderVec3(mapping.MATERIAL_SPECULAR, specular);
	setShaderVec3(mapping.MATERIAL_EMISSIVE, emissive);
	setShaderVec3(mapping.MATERIAL_AMBIENT, ambient);
	setShaderVec3(mapping.MATERIAL_DIFFUSE_PROPERTY, diffuseProperty);
	setShaderVec3(mapping.MATERIAL_SPECULAR_PROPERTY, specularProperty);
	setShaderFloat(mapping.MATERIAL_SHININESS, shininess);
	setShaderFloat(mapping.MATERIAL_OPACITY, transparency);
	setShaderBVec4(mapping.MATERIAL_USE_TEXTURE, textureList.useTexture[0], textureList.useTexture[1], textureList.useTexture[2], textureList.useTexture[3]);
	textureList.render(mapping);
}
