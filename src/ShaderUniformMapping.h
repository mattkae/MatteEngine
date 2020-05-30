#pragma once
#include "Shader.h"
#include "Constants.h"

namespace ShaderUniformMapping {
	namespace ModelShader {
		extern int DIFFUSE_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH];
		extern int SPECULAR_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH];
		extern int AMBIENT_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH];
		extern int NORMAL_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH];
	};

	void initialize(const Shader& shader);
};