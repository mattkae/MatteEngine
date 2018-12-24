#ifndef SHADER_H
#define SHADER_H
#include "gl_includes.h"
#include <glm/glm.hpp>

class Shader {
 public:
  Shader();
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath = nullptr);
  void load(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geomPath = nullptr);
  void use();
  GLuint get_program();

  void set_uniform_1f(const GLchar* name, GLfloat v0);
  void set_uniform_2f(const GLchar* name, GLfloat v0, GLfloat v1);
  void set_uniform_3f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);
  void set_uniform_4f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
  void set_uniform_1i(const GLchar* name, GLint v0);
  void set_uniform_2i(const GLchar* name, GLint v0, GLint v1);
  void set_uniform_3i(const GLchar* name, GLint v0, GLint v1, GLint v2);
  void set_uniform_4i(const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3);
  void set_uniform_1ui(const GLchar* name, GLuint v0);
  void set_uniform_2ui(const GLchar* name, GLuint v0, GLuint v1);
  void set_uniform_3ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2);
  void set_uniform_4ui(const GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
  void set_uniform_1fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void set_uniform_2fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void set_uniform_3fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void set_uniform_4fv(const GLchar* name, GLsizei count, const GLfloat *v0);
  void set_uniform_1iv(const GLchar* name, GLsizei count, const GLint *v0);
  void set_uniform_2iv(const GLchar* name, GLsizei count, const GLint *v0);
  void set_uniform_3iv(const GLchar* name, GLsizei count, const GLint *v0);
  void set_uniform_4iv(const GLchar* name, GLsizei count, const GLint *v0);
  void set_uniform_1uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void set_uniform_2uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void set_uniform_3uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void set_uniform_4uiv(const GLchar* name, GLsizei count, const GLuint *v0);
  void set_uniform_matrix_2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_2x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_3x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_2x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_4x2fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_3x4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);
  void set_uniform_matrix_4x3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* v0);

 private:
  GLint get_uniform(const GLchar* name);
  GLuint mProgram;
};
#endif
