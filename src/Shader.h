#ifndef SHADER_H
#define SHADER_H
//#include "gl_includes.h"
#include <GL/glew.h>
class Shader {
 public:
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
  Shader* Use();
  GLuint GetProgram();

  void SetUniform1f(GLchar* name, GLfloat v0);
  void SetUniform2f(GLchar* name, GLfloat v0, GLfloat v1);
  void SetUniform3f(GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);
  void SetUniform4f(GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
  void SetUniform1i(GLchar* name, GLint v0);
  void SetUniform2i(GLchar* name, GLint v0, GLint v1);
  void SetUniform3i(GLchar* name, GLint v0, GLint v1, GLint v2);
  void SetUniform4i(GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3);
  void SetUniform1ui(GLchar* name, GLuint v0);
  void SetUniform2ui(GLchar* name, GLuint v0, GLuint v1);
  void SetUniform3ui(GLchar* name, GLuint v0, GLuint v1, GLuint v2);
  void SetUniform4ui(GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
  void SetUniform1fv(GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform2fv(GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform3fv(GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform4fv(GLchar* name, GLsizei count, const GLfloat *v0);
  void SetUniform1iv(GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform2iv(GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform3iv(GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform4iv(GLchar* name, GLsizei count, const GLint *v0);
  void SetUniform1uiv(GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniform2uiv(GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniform3uiv(GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniform4uiv(GLchar* name, GLsizei count, const GLuint *v0);
  void SetUniformMatrix2fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix3fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix4fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix2x3fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix3x2fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix2x4fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix4x2fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix3x4fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void SetUniformMatrix4x3fv(GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);

 private:
  GLint GetUniform(const GLchar* name);
  GLuint Program;
};
#endif
