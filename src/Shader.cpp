#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() {}

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath,
               const GLchar *geomPath) {
    this->load(vertexPath, fragmentPath, geomPath);
}

GLuint loadShader(GLenum shaderType, const GLchar *path) {
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderCode = shaderStream.str();
    } catch (std::exception e) {
        std::cerr << "Error: Shader not successfuly read from file: " << path
                  << std::endl;
        return 0;
    }

    const GLchar *glShaderCode = shaderCode.c_str();
    GLuint shader;

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &glShaderCode, 0);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, 0, infoLog);
        std::cerr << "Error: Vertex shader failed to compile - " << infoLog
                  << std::endl;
        return 0;
    }

    return shader;
}

void Shader::load(const GLchar *vertexPath, const GLchar *fragmentPath,
                  const GLchar *geomPath) {
    this->mProgram = glCreateProgram();

    GLuint vertex, fragment, geometry;
    if (vertexPath) {
        vertex = loadShader(GL_VERTEX_SHADER, vertexPath);
        glAttachShader(this->mProgram, vertex);
    }

    if (fragmentPath) {
        fragment = loadShader(GL_FRAGMENT_SHADER, fragmentPath);
        glAttachShader(this->mProgram, fragment);
    }

    if (geomPath) {
        geometry = loadShader(GL_GEOMETRY_SHADER, geomPath);
        glAttachShader(this->mProgram, geometry);
    }

    glLinkProgram(this->mProgram);
    GLint isLinked = 0;
    glGetProgramiv(this->mProgram, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(this->mProgram, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(this->mProgram, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(this->mProgram);
        printf("Error. Could not initialize shader with vertex=%s\n",
               vertexPath);
    }

    if (vertexPath)
        glDeleteShader(vertex);
    if (fragmentPath)
        glDeleteShader(fragment);
    if (geomPath)
        glDeleteShader(geometry);
}

void Shader::use() const { glUseProgram(this->mProgram); }

GLuint Shader::get_program() { return this->mProgram; }

GLint Shader::get_uniform(const GLchar *name) const {
    return glGetUniformLocation(this->mProgram, name);
}

void Shader::set_uniform_1f(const GLchar *name, GLfloat v0) const {
    glUniform1f(this->get_uniform(name), v0);
}

void Shader::set_uniform_2f(const GLchar *name, GLfloat v0, GLfloat v1) const {
    glUniform2f(this->get_uniform(name), v0, v1);
}

void Shader::set_uniform_3f(const GLchar *name, GLfloat v0, GLfloat v1,
                            GLfloat v2) const {
    glUniform3f(this->get_uniform(name), v0, v1, v2);
}

void Shader::set_uniform_4f(const GLchar *name, GLfloat v0, GLfloat v1,
                            GLfloat v2, GLfloat v3) const {
    glUniform4f(this->get_uniform(name), v0, v1, v2, v3);
}

void Shader::set_uniform_1i(const GLchar *name, GLint v0) const {
    glUniform1i(this->get_uniform(name), v0);
}

void Shader::set_uniform_2i(const GLchar *name, GLint v0, GLint v1) const {
    glUniform2i(this->get_uniform(name), v0, v1);
}

void Shader::set_uniform_3i(const GLchar *name, GLint v0, GLint v1,
                            GLint v2) const {
    glUniform3i(this->get_uniform(name), v0, v1, v2);
}

void Shader::set_uniform_4i(const GLchar *name, GLint v0, GLint v1, GLint v2,
                            GLint v3) const {
    glUniform4i(this->get_uniform(name), v0, v1, v2, v3);
}

void Shader::set_uniform_1ui(const GLchar *name, GLuint v0) const {
    glUniform1ui(this->get_uniform(name), v0);
}

void Shader::set_uniform_2ui(const GLchar *name, GLuint v0, GLuint v1) const {
    glUniform2ui(this->get_uniform(name), v0, v1);
}

void Shader::set_uniform_3ui(const GLchar *name, GLuint v0, GLuint v1,
                             GLuint v2) const {
    glUniform3ui(this->get_uniform(name), v0, v1, v2);
}

void Shader::set_uniform_4ui(const GLchar *name, GLuint v0, GLuint v1,
                             GLuint v2, GLuint v3) const {
    glUniform4ui(this->get_uniform(name), v0, v1, v2, v3);
}

void Shader::set_uniform_1fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) const {
    glUniform1fv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_2fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) const {
    glUniform2fv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_3fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) const {
    glUniform3fv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_4fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) const {
    glUniform4fv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_1iv(const GLchar *name, GLsizei count,
                             const GLint *v0) const {
    glUniform1iv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_2iv(const GLchar *name, GLsizei count,
                             const GLint *v0) const {
    glUniform2iv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_3iv(const GLchar *name, GLsizei count,
                             const GLint *v0) const {
    glUniform3iv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_4iv(const GLchar *name, GLsizei count,
                             const GLint *v0) const {
    glUniform4iv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_1uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) const {
    glUniform1uiv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_2uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) const {
    glUniform2uiv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_3uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) const {
    glUniform3uiv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_4uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) const {
    glUniform4uiv(this->get_uniform(name), count, v0);
}

void Shader::set_uniform_matrix_2fv(const GLchar *name, GLsizei count,
                                    GLboolean transpose,
                                    const GLfloat *v0) const {
    glUniformMatrix2fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_3fv(const GLchar *name, GLsizei count,
                                    GLboolean transpose,
                                    const GLfloat *v0) const {
    glUniformMatrix3fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_4fv(const GLchar *name, GLsizei count,
                                    GLboolean transpose,
                                    const GLfloat *v0) const {
    glUniformMatrix4fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_2x3fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose,
                                      const GLfloat *v0) const {
    glUniformMatrix2x3fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_3x2fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose,
                                      const GLfloat *v0) const {
    glUniformMatrix3x2fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_2x4fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose,
                                      const GLfloat *v0) const {
    glUniformMatrix2x4fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_4x2fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose,
                                      const GLfloat *v0) const {
    glUniformMatrix4x2fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_3x4fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose,
                                      const GLfloat *v0) const {
    glUniformMatrix3x4fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::set_uniform_matrix_4x3fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose,
                                      const GLfloat *v0) const {
    glUniformMatrix4x3fv(this->get_uniform(name), count, transpose, v0);
}

void Shader::setFloat(const GLchar* name, GLfloat value) const {
    glUniform1f(this->get_uniform(name), value);
}

void Shader::setInt(const GLchar* name, GLint value) const {
    glUniform1i(this->get_uniform(name), value);
}

void Shader::setUint(const GLchar* name, GLuint value) const {
    glUniform1ui(this->get_uniform(name), value);
}

void Shader::setVec2(const GLchar* name, glm::vec2 value) const {
    glUniform2f(this->get_uniform(name), value.x, value.y);
}

void Shader::setVec3(const GLchar* name, glm::vec3 value) const {
    glUniform3f(this->get_uniform(name), value.x, value.y, value.z);
}

void Shader::setVec4(const GLchar* name, glm::vec4 value) const {
    glUniform4f(this->get_uniform(name), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const GLchar* name, glm::mat4 matrix) const {
    glUniformMatrix4fv(this->get_uniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat3(const GLchar* name, glm::mat3 matrix) const {
    glUniformMatrix3fv(this->get_uniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
}