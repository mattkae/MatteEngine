#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() {
  
}

/*********************************************
 	Create a shader program from the given shader paths.

 	@param vertexPath - Path to vertex shader.
 	@param fragmentPath - Path to fragment shader.
*********************************************/
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
  this->load(vertexPath, fragmentPath);
}

/*********************************************
Creates a shader from the given vertex and fragment programs
 ********************************************/
void Shader::load(const GLchar* vertexPath, const GLchar* fragmentPath)
{
  // Read the shader from file path.
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure e) {
    std::cerr << "Error: Shader file not successfully read" << std::endl;
  }

  const GLchar* vShaderCode = vertexCode.c_str();
  const GLchar* fShaderCode = fragmentCode.c_str();

  // Compile the shader.
  GLuint vertex, fragment;
  GLint success;
  GLchar infoLog[512];

  // Vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, 0);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, 0, infoLog);
    std::cerr << "Error: Vertex shader failed to compile - " << infoLog << std::endl;
  }

  // Fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, 0);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, 0, infoLog);
    std::cerr << "Error: Fragment shader failed to compile - " << infoLog << std::endl;
  }

  // Shader program
  this->Program = glCreateProgram();
  glAttachShader(this->Program, vertex);
  glAttachShader(this->Program, fragment);
  glLinkProgram(this->Program);
  glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->Program, 512, 0, infoLog);
    std::cerr << "Error: Shader program failed to link - " << infoLog << std::endl;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

/*********************************************
		Use this shader's program.

		@return this - Shader object
*********************************************/
void
Shader::Use() const
{
  glUseProgram(this->Program);
}

/*********************************************
		Returns the shader's program number.

		@return shader program.
*********************************************/
GLuint
Shader::GetProgram() const
{
  return this->Program;
}

/*********************************************
		Return the location of the given name.

		@param name - Tag being looked for.

		@return location of uniform.
*********************************************/
GLint
Shader::GetUniform(const GLchar* name) const
{
  return glGetUniformLocation(this->Program, name);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
*********************************************/
void
Shader::SetUniform1f(const GLchar* name, GLfloat v0) const
{
  glUniform1f(this->GetUniform(name), v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
*********************************************/
void
Shader::SetUniform2f(const GLchar* name, GLfloat v0, GLfloat v1) const
{
  glUniform2f(this->GetUniform(name), v0, v1);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
		@param v2 - Third value.
*********************************************/
void
Shader::SetUniform3f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2) const
{
  glUniform3f(this->GetUniform(name), v0, v1, v2);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
		@param v2 - Third value.
		@param v3 - Fourth value.
*********************************************/
void
Shader::SetUniform4f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
  const
{
  glUniform4f(this->GetUniform(name), v0, v1, v2, v3);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
*********************************************/
void
Shader::SetUniform1i(const GLchar* name, GLint v0) const
{
  glUniform1i(this->GetUniform(name), v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
*********************************************/
void
Shader::SetUniform2i(const GLchar* name, GLint v0, GLint v1) const
{
  glUniform2i(this->GetUniform(name), v0, v1);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
		@param v2 - Third value.
*********************************************/
void
Shader::SetUniform3i(const GLchar* name, GLint v0, GLint v1, GLint v2) const
{
  glUniform3i(this->GetUniform(name), v0, v1, v2);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
		@param v2 - Third value.
		@param v3 - Fourth value.
*********************************************/
void
Shader::SetUniform4i(const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3) const
{
  glUniform4i(this->GetUniform(name), v0, v1, v2, v3);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
*********************************************/
void
Shader::SetUniform1ui(const GLchar* name, GLuint v0) const
{
  glUniform1ui(this->GetUniform(name), v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
*********************************************/
void
Shader::SetUniform2ui(const GLchar* name, GLuint v0, GLuint v1) const
{
  glUniform2ui(this->GetUniform(name), v0, v1);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
		@param v2 - Third value.
*********************************************/
void
Shader::SetUniform3ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2) const
{
  glUniform3ui(this->GetUniform(name), v0, v1, v2);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param v0 - First value.
		@param v1 - Second value.
		@param v2 - Third value.
		@param v3 - Fourth value.
*********************************************/
void
Shader::SetUniform4ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3) const
{
  glUniform4ui(this->GetUniform(name), v0, v1, v2, v3);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform1fv(const GLchar* name, GLsizei count, const GLfloat *v0) const
{
  glUniform1fv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform2fv(const GLchar* name, GLsizei count, const GLfloat *v0) const
{
  glUniform2fv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform3fv(const GLchar* name, GLsizei count, const GLfloat *v0) const
{
  glUniform3fv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform4fv(const GLchar* name, GLsizei count, const GLfloat *v0) const
{
  glUniform4fv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform1iv(const GLchar* name, GLsizei count, const GLint *v0) const
{
  glUniform1iv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform2iv(const GLchar* name, GLsizei count, const GLint *v0) const
{
  glUniform2iv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform3iv(const GLchar* name, GLsizei count, const GLint *v0) const
{
  glUniform3iv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform4iv(const GLchar* name, GLsizei count, const GLint *v0) const
{
  glUniform4iv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform1uiv(const GLchar* name, GLsizei count, const GLuint *v0) const
{
  glUniform1uiv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform2uiv(const GLchar* name, GLsizei count, const GLuint *v0) const
{
  glUniform2uiv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform3uiv(const GLchar* name, GLsizei count, const GLuint *v0) const
{
  glUniform3uiv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param v0
*********************************************/
void
Shader::SetUniform4uiv(const GLchar* name, GLsizei count, const GLuint *v0) const
{
  glUniform4uiv(this->GetUniform(name), count, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix2fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix3fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix4fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix2x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix2x3fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix3x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix3x2fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix2x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix2x4fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix4x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix4x2fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix3x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix3x4fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
		Set the value of the given tag in the shader.

		@param name - Tag being looked for.
		@param count
		@param transpose
		@param v0
*********************************************/
void
Shader::SetUniformMatrix4x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const
{
  glUniformMatrix4x3fv(this->GetUniform(name), count, transpose, v0);
}
