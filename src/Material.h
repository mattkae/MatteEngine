#ifndef MATERIAL_H
#define MATERIAL_H
#include "gl_includes.h"
#include <glm/glm.hpp>  

struct Material {
  Material();
  Material(const Material& other);
  Material(glm::vec4 emissive, glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular);
  Material(glm::vec4 emissive, glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular, GLfloat shininess);
  void render();
  void set_diffuse(float r, float g, float b, float a);
  void set_specular(float r, float g, float b, float a);
  void set_ambient(float r, float g, float b, float a);
  void set_emissive(float r, float g, float b, float a);
  
 private:
  glm::vec4 mEmissive;
  glm::vec4 mDiffuse;
  glm::vec4 mAmbient;
  glm::vec4 mSpecular;
  GLfloat mShininess = 0;
};

#endif
