#ifndef SHADER_H
#define SHADER_H
#include "gl_includes.h"

class Shader {
 public:
  Shader();
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath = nullptr);
  void load(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath = nullptr);
  void Use() const;
  GLuint GetProgram() const;

  void SetUniform1f(const GLchar* name, GLfloat v0) const;
  void SetUniform2f(const GLchar* name, GLfloat v0, GLfloat v1) const;
  void SetUniform3f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2) const;
  void SetUniform4f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
  void SetUniform1i(const GLchar* name, GLint v0) const;
  void SetUniform2i(const GLchar* name, GLint v0, GLint v1) const;
  void SetUniform3i(const GLchar* name, GLint v0, GLint v1, GLint v2) const;
  void SetUniform4i(const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3) const;
  void SetUniform1ui(const GLchar* name, GLuint v0) const;
  void SetUniform2ui(const GLchar* name, GLuint v0, GLuint v1) const;
  void SetUniform3ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2) const;
  void SetUniform4ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3) const;
  void SetUniform1fv(const GLchar* name, GLsizei count, const GLfloat *v0) const;
  void SetUniform2fv(const GLchar* name, GLsizei count, const GLfloat *v0) const;
  void SetUniform3fv(const GLchar* name, GLsizei count, const GLfloat *v0) const;
  void SetUniform4fv(const GLchar* name, GLsizei count, const GLfloat *v0) const;
  void SetUniform1iv(const GLchar* name, GLsizei count, const GLint *v0) const;
  void SetUniform2iv(const GLchar* name, GLsizei count, const GLint *v0) const;
  void SetUniform3iv(const GLchar* name, GLsizei count, const GLint *v0) const;
  void SetUniform4iv(const GLchar* name, GLsizei count, const GLint *v0) const;
  void SetUniform1uiv(const GLchar* name, GLsizei count, const GLuint *v0) const;
  void SetUniform2uiv(const GLchar* name, GLsizei count, const GLuint *v0) const;
  void SetUniform3uiv(const GLchar* name, GLsizei count, const GLuint *v0) const;
  void SetUniform4uiv(const GLchar* name, GLsizei count, const GLuint *v0) const;
  void SetUniformMatrix2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix2x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix3x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix2x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix4x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix3x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;
  void SetUniformMatrix4x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0) const;

 private:
  GLint GetUniform(const GLchar* name) const;
  GLuint Program;
};
#endif
