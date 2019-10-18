#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Matrix4x4f.h"
#include "Matrix3x3f.h"
#include "Vector4f.h"

typedef GLuint Shader;

Shader loadShader(const GLchar* vertexPath, const GLchar* fragmentPath = nullptr, const GLchar* geomPath = nullptr);

inline GLint getShaderUniform(const Shader& shader, const GLchar *name) {
    return glGetUniformLocation(shader, name);
}

inline void useShader(const Shader& shader) {
	glUseProgram(shader);
}

inline void setShaderFloat(const Shader& shader, const GLchar* name, GLfloat value) {
    glUniform1f(getShaderUniform(shader, name), value);
}

inline void setShaderInt(const Shader& shader, const GLchar* name, GLint value) {
	glUniform1i(getShaderUniform(shader, name), value);
}

inline void setShaderUint(const Shader& shader, const GLchar* name, GLuint value) {
	glUniform1ui(getShaderUniform(shader, name), value);
}

inline void setShaderVec2(const Shader& shader, const GLchar* name, glm::vec2 value) {
	glUniform2f(getShaderUniform(shader, name), value.x, value.y);
}

inline void setShaderVec3(const Shader& shader, const GLchar* name, glm::vec3 value) {
	glUniform3f(getShaderUniform(shader, name), value.x, value.y, value.z);
}

inline void setShaderVec4(const Shader& shader, const GLchar* name, glm::vec4 value) {
	glUniform4f(getShaderUniform(shader, name), value.x, value.y, value.z, value.w);
}

inline void setShaderMat3(const Shader& shader, const GLchar* name, glm::mat3 matrix) {
	glUniformMatrix3fv(getShaderUniform(shader, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

inline void setShaderMat3(const Shader& shader, const GLchar* name, const Matrix3x3f& matrix) {
	glUniformMatrix3fv(getShaderUniform(shader, name), 1, GL_FALSE, matrix.values);
}

inline void setShaderMat4(const Shader& shader, const GLchar* name, glm::mat4 matrix) {
	glUniformMatrix4fv(getShaderUniform(shader, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

inline void setShaderMat4(const Shader& shader, const GLchar* name, const Matrix4x4f& matrix) {
	glUniformMatrix4fv(getShaderUniform(shader, name), 1, GL_FALSE, matrix.values);
}

inline void setShaderVec3(const Shader& shader, const GLchar* name, Vector3f value) {
	glUniform3f(getShaderUniform(shader, name), value.x, value.y, value.z);
}
#endif
