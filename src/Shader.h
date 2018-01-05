#ifndef SHADER_H
#define SHADER_H
#include "gl_includes.h"

class Shader {
 public:
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
  Shader* Use();
  GLuint GetProgram();

  void SetUniform1f(const GLchar* name, GLfloat v0);
  void SetUniform2f(const GLchar* name, GLfloat v0, GLfloat v1);
  void SetUniform3f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);
  void SetUniform4f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
  void SetUniform1i(const GLchar* name, GLint v0);
  void SetUniform2i(const GLchar* name, GLint v0, GLint v1);
  void SetUniform3i(const GLchar* name, GLint v0, GLint v1, GLint v2);
  void SetUniform4i(const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3);
  void SetUniform1ui(const GLchar* name, GLuint v0);
  void SetUniform2ui(const GLchar* name, GLuint v0, GLuint v1);
  void SetUniform3ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2);
  void SetUniform4ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
  void SetUniform1fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform2fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform3fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform4fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform1iv(const GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform2iv(const GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform3iv(const GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform4iv(const GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform1uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniform2uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniform3uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniform4uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniformMatrix2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix2x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix3x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix2x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix4x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix3x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix4x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);

 private:
  GLint GetUniform(const GLchar* name);
  GLuint Program;
};
#endif
