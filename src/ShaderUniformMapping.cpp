#include "ShaderUniformMapping.h"

int ShaderUniformMapping::ModelShader::DIFFUSE_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };
int ShaderUniformMapping::ModelShader::SPECULAR_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };
int ShaderUniformMapping::ModelShader::AMBIENT_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };
int ShaderUniformMapping::ModelShader::NORMAL_LIST[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 0, 0, 0 };

void ShaderUniformMapping::initialize(const Shader& shader) {
	ModelShader::DIFFUSE_LIST[0] = getShaderUniform(shader, "uDiffuseList[0]");
	ModelShader::DIFFUSE_LIST[1] = getShaderUniform(shader, "uDiffuseList[1]");
	ModelShader::DIFFUSE_LIST[2] = getShaderUniform(shader, "uDiffuseList[2]");

	ModelShader::SPECULAR_LIST[0] = getShaderUniform(shader, "uSpecularList[0]");
	ModelShader::SPECULAR_LIST[1] = getShaderUniform(shader, "uSpecularList[1]");
	ModelShader::SPECULAR_LIST[2] = getShaderUniform(shader, "uSpecularList[2]");

	ModelShader::AMBIENT_LIST[0] = getShaderUniform(shader, "uAmbientList[0]");
	ModelShader::AMBIENT_LIST[1] = getShaderUniform(shader, "uAmbientList[1]");
	ModelShader::AMBIENT_LIST[2] = getShaderUniform(shader, "uAmbientList[2]");

	/*
	@TODO: Turn these on once they're being used!
	ModelShader::NORMAL_LIST[0] = getShaderUniform(shader, "uNormalMapList[0]");
	ModelShader::NORMAL_LIST[1] = getShaderUniform(shader, "uNormalMapList[1]");
	ModelShader::NORMAL_LIST[2] = getShaderUniform(shader, "uNormalMapList[2]");
	*/
}