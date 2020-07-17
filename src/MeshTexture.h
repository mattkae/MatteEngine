#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "Constants.h"
#include "ShaderUniformMapping.h"

enum class TextureType {
	NONE,
	DIFFUSE,
	AMBIENT,
	SPECULAR,
	NORMAL,
	MAX_VALUE
};

struct MeshTexture {
	GLint diffuse[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };
	GLint specular[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };
	GLint ambient[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };
	GLint normal[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };
	bool useTexture[4]  = { false, false, false, false };

	void add(TextureType type, GLuint texture);
	void render(const MaterialUniformMapping& mapping) const;
};