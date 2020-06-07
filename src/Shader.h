#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include <vector>
#include "Matrix4x4f.h"
#include "Matrix3x3f.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Logger.h"

typedef GLuint Shader;

Shader loadShader(const GLchar* vertexPath, const GLchar* fragmentPath = nullptr, const GLchar* geomPath = nullptr);
Shader reloadShader(Shader shader);
void watchForDirectorychanges(std::vector<Shader>& shadersToReload, const bool& isDying);

inline GLint getShaderUniform(const Shader& shader, const GLchar *name) {
    GLint uid = glGetUniformLocation(shader, name);
	if (uid < 0) {
		Logger::logError("Failed to find uniform for: " + std::string(name));
	}
	return uid;
}

inline void useShader(const Shader& shader) {
	glUseProgram(shader);
}

inline void setShaderFloat(GLint location, GLfloat value) {
    glUniform1f(location, value);
}

inline void setShaderInt(GLint location, GLint value) {
	glUniform1i(location, value);
}

inline void setShaderUint(GLint location, GLuint value) {
	glUniform1ui(location, value);
}

inline void setShaderVec2(GLint location, const Vector2f& value) {
	glUniform2f(location, value.x, value.y);
}

inline void setShaderVec4(GLint location, const Vector4f& value) {
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

inline void setShaderMat3(GLint location, const Matrix3x3f& matrix) {
	glUniformMatrix3fv(location, 1, GL_FALSE, matrix.values);
}

inline void setShaderMat4(GLint location, const Matrix4x4f& matrix) {
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix.values);
}

inline void setShaderMat4Multiple(GLint location, GLuint size, const Matrix4x4f* matrices) {
	glUniformMatrix4fv(location, size, GL_FALSE, (const GLfloat*)matrices);
}

inline void setShaderVec3(GLint location, const Vector3f& value) {
	glUniform3f(location, value.x, value.y, value.z);
}

inline void setShaderBVec3(GLint location, bool first, bool second, bool third) {
	glUniform3i(location, first, second, third);
}

inline void setShaderBool(GLint location, bool value) {
	glUniform1i(location, value);
}
#endif
