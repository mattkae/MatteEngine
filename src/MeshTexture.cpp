#pragma once
#include "MeshTexture.h"
#include "MyString.h"
#include "TextureUniformConstants.h"

inline void renderTexture(GLint index, GLuint textureHandle, const GLint uniform) {
 	setShaderInt(uniform, index);
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
		diffuse = texture;
		break;
	case TextureType::AMBIENT:
		useTexture[2] = true;
		ambient = texture;
		break;
	case TextureType::SPECULAR:
		useTexture[1] = true;
		specular = texture;
		break;
	case TextureType::NORMAL:
		useTexture[3] = true;
		normal = texture;
		break;
	}
}

void MeshTexture::render(const MaterialUniformMapping& mapping) const {
    renderTexture(TextureUniformConstants::DIFFUSE_TEXTURE_POSITION, diffuse, mapping.DIFFUSE);
	renderTexture(TextureUniformConstants::SPECULAR_TEXTURE_POSITION, specular, mapping.SPECULAR);
	renderTexture(TextureUniformConstants::NORMAL_TEXTURE_POSITION, normal, mapping.NORMAL);
	renderTexture(TextureUniformConstants::AMBIENT_TEXTURE_POSITION, ambient, mapping.AMBIENT);
}
