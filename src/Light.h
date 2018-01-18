#ifndef LIGHT_H
#define LIGHT_H

#include "gl_includes.h"

class Shader;

enum LightType {
  Directional = 0,
  Point,
  Spot,
  Inactive
};

class Light {
 public:
  glm::vec3 direction = glm::vec3(0.0, 0.0, -1.0);
  glm::vec3 position = glm::vec3(0.0);
  glm::vec3 color = glm::vec3(1.0);
  float constant = 1.f;
  float linear = 0.f;
  float quadratic = 0.f;
  float cosineCutOff = 0.99f;
  float dropOff = 0.f;
  LightType type = Directional;
  bool isOn = true;
  glm::mat4 view;
  glm::mat4 projection;

  void render(Shader* shader);
 private:
  GLuint mDepthTexture = 0;
  GLuint mDepthFbo = 0;
};

#endif
