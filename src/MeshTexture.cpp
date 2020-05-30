#pragma once
#include "MeshTexture.h"
#include "String.h"
#include "TextureUniformConstants.h"
#include "ShaderUniformMapping.h"

inline void renderTexture(GLint index, GLuint textureHandle, const GLint uniform) {
 	setShaderIntWithUniform(uniform, index);
	if (textureHandle > 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
	}
}

void MeshTexture::add(TextureType type, GLuint texture) {
	GLint* textureMapping = nullptr;
	switch (type) {
	case TextureType::NONE:
		break;
	case TextureType::DIFFUSE:
		useTexture[0] = true;
		textureMapping = diffuse;
		break;
	case TextureType::AMBIENT:
		useTexture[2] = true;
		textureMapping = ambient;
		break;
	case TextureType::SPECULAR:
		useTexture[1] = true;
		textureMapping = specular;
		break;
	case TextureType::NORMAL:
		textureMapping = normal;
		break;
	}

	if (textureMapping != nullptr) {
		for (int textureIndex = 0; textureIndex < Constants::Rendering::MAX_TEXTURES_PER_MESH; textureIndex++) {
			if (textureMapping[textureIndex] == 0) {
				textureMapping[textureIndex] = texture;
				break;
			}
		}
	}
}

void MeshTexture::render(const Shader& shader) const {
	for (int textureIndex = 0; textureIndex < Constants::Rendering::MAX_TEXTURES_PER_MESH; textureIndex++) {
		renderTexture(TextureUniformConstants::DIFFUSE_TEXTURE_POSTIION + textureIndex, diffuse[textureIndex], ShaderUniformMapping::ModelShader::DIFFUSE_LIST[textureIndex]);
	}
	for (int textureIndex = 0; textureIndex < Constants::Rendering::MAX_TEXTURES_PER_MESH; textureIndex++) {
		renderTexture(TextureUniformConstants::SPECULAR_TEXTURE_POSTIION + textureIndex, specular[textureIndex], ShaderUniformMapping::ModelShader::SPECULAR_LIST[textureIndex]);
	}
	for (int textureIndex = 0; textureIndex < Constants::Rendering::MAX_TEXTURES_PER_MESH; textureIndex++) {
		renderTexture(TextureUniformConstants::AMBIENT_TEXTURE_POSITION + textureIndex, ambient[textureIndex], ShaderUniformMapping::ModelShader::AMBIENT_LIST[textureIndex]);
	}
	/*for (int textureIndex = 0; textureIndex < Constants::Rendering::MAX_TEXTURES_PER_MESH; textureIndex++) {
		renderTexture(TextureUniformConstants::NORMAL_TEXTURE_POSITION + textureIndex, normal[textureIndex], ShaderUniformMapping::ModelShader::NORMAL_LIST[textureIndex]);
	}*/
}