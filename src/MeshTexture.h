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
	GLint diffuse = 0;
	GLint specular = 0;
	GLint ambient = 0;
	GLint normal = 0;
	bool useTexture[4]  = { false, false, false, false };

	void add(TextureType type, GLuint texture);
	void render(const MaterialUniformMapping& mapping) const;
};
