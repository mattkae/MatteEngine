#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Shader::Shader() {}

/*********************************************
        Create a shader program from the given shader paths.

        @param vertexPath - Path to vertex shader.
        @param fragmentPath - Path to fragment shader.
*********************************************/
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

/*********************************************
Creates a shader from the given vertex and fragment programs
 ********************************************/
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

/*********************************************
                use this shader's program.

                @return this - Shader object
*********************************************/
void Shader::use() { glUseProgram(this->mProgram); }

/*********************************************
                Returns the shader's program number.

                @return shader program.
*********************************************/
GLuint Shader::get_program() { return this->mProgram; }

/*********************************************
                Return the location of the given name.

                @param name - Tag being looked for.

                @return location of uniform.
*********************************************/
GLint Shader::get_uniform(const GLchar *name) {
    return glGetUniformLocation(this->mProgram, name);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
*********************************************/
void Shader::set_uniform_1f(const GLchar *name, GLfloat v0) {
    glUniform1f(this->get_uniform(name), v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
*********************************************/
void Shader::set_uniform_2f(const GLchar *name, GLfloat v0, GLfloat v1) {
    glUniform2f(this->get_uniform(name), v0, v1);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
                @param v2 - Third value.
*********************************************/
void Shader::set_uniform_3f(const GLchar *name, GLfloat v0, GLfloat v1,
                            GLfloat v2) {
    glUniform3f(this->get_uniform(name), v0, v1, v2);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
                @param v2 - Third value.
                @param v3 - Fourth value.
*********************************************/
void Shader::set_uniform_4f(const GLchar *name, GLfloat v0, GLfloat v1,
                            GLfloat v2, GLfloat v3) {
    glUniform4f(this->get_uniform(name), v0, v1, v2, v3);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
*********************************************/
void Shader::set_uniform_1i(const GLchar *name, GLint v0) {
    glUniform1i(this->get_uniform(name), v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
*********************************************/
void Shader::set_uniform_2i(const GLchar *name, GLint v0, GLint v1) {
    glUniform2i(this->get_uniform(name), v0, v1);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
                @param v2 - Third value.
*********************************************/
void Shader::set_uniform_3i(const GLchar *name, GLint v0, GLint v1, GLint v2) {
    glUniform3i(this->get_uniform(name), v0, v1, v2);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
                @param v2 - Third value.
                @param v3 - Fourth value.
*********************************************/
void Shader::set_uniform_4i(const GLchar *name, GLint v0, GLint v1, GLint v2,
                            GLint v3) {
    glUniform4i(this->get_uniform(name), v0, v1, v2, v3);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
*********************************************/
void Shader::set_uniform_1ui(const GLchar *name, GLuint v0) {
    glUniform1ui(this->get_uniform(name), v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
*********************************************/
void Shader::set_uniform_2ui(const GLchar *name, GLuint v0, GLuint v1) {
    glUniform2ui(this->get_uniform(name), v0, v1);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
                @param v2 - Third value.
*********************************************/
void Shader::set_uniform_3ui(const GLchar *name, GLuint v0, GLuint v1,
                             GLuint v2) {
    glUniform3ui(this->get_uniform(name), v0, v1, v2);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param v0 - First value.
                @param v1 - Second value.
                @param v2 - Third value.
                @param v3 - Fourth value.
*********************************************/
void Shader::set_uniform_4ui(const GLchar *name, GLuint v0, GLuint v1,
                             GLuint v2, GLuint v3) {
    glUniform4ui(this->get_uniform(name), v0, v1, v2, v3);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_1fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) {
    glUniform1fv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_2fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) {
    glUniform2fv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_3fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) {
    glUniform3fv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_4fv(const GLchar *name, GLsizei count,
                             const GLfloat *v0) {
    glUniform4fv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_1iv(const GLchar *name, GLsizei count,
                             const GLint *v0) {
    glUniform1iv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_2iv(const GLchar *name, GLsizei count,
                             const GLint *v0) {
    glUniform2iv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_3iv(const GLchar *name, GLsizei count,
                             const GLint *v0) {
    glUniform3iv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_4iv(const GLchar *name, GLsizei count,
                             const GLint *v0) {
    glUniform4iv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_1uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) {
    glUniform1uiv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_2uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) {
    glUniform2uiv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_3uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) {
    glUniform3uiv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param v0
*********************************************/
void Shader::set_uniform_4uiv(const GLchar *name, GLsizei count,
                              const GLuint *v0) {
    glUniform4uiv(this->get_uniform(name), count, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_2fv(const GLchar *name, GLsizei count,
                                    GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix2fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_3fv(const GLchar *name, GLsizei count,
                                    GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix3fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_4fv(const GLchar *name, GLsizei count,
                                    GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix4fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_2x3fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix2x3fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_3x2fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix3x2fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_2x4fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix2x4fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_4x2fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix4x2fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_3x4fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix3x4fv(this->get_uniform(name), count, transpose, v0);
}

/*********************************************
                Set the value of the given tag in the shader.

                @param name - Tag being looked for.
                @param count
                @param transpose
                @param v0
*********************************************/
void Shader::set_uniform_matrix_4x3fv(const GLchar *name, GLsizei count,
                                      GLboolean transpose, const GLfloat *v0) {
    glUniformMatrix4x3fv(this->get_uniform(name), count, transpose, v0);
}
