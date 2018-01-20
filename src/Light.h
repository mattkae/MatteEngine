#ifndef LIGHT_H
#define LIGHT_H

#include "gl_includes.h"
#include <glm/glm.hpp>

class Shader;

enum LightType {
  Directional = 0,
  Point,
  Spot,
  Inactive
};

struct Light {
  LightType type = Directional;
  bool isOn = true;
  glm::vec3 color = glm::vec3(1.0);
  glm::vec3 direction = glm::vec3(0.0, 0.0, 1.0);
  glm::vec3 position = glm::vec3(0.0);

  // Attenuation
  float constant = 1.f;
  float linear = 0.f;
  float quadratic = 0.f;

  // Spot of light
  float cosineCutOff = 0.99f;
  float dropOff = 0.f;

  // Shadow variables
  glm::mat4 view = glm::mat4(0.0);
  glm::mat4 projection = glm::mat4(0.0);
  GLuint shadowTexture = 0;
  GLuint depthFbo = 0;
  bool usesShadows = false;
};

void allocate(Light* light, int width, int height);
void gen_shadow_texture(Shader* shader, Light light, int width, int height);
void render_light(Shader* shader, Light light, int index);
Light get_directional(glm::vec3 color, glm::vec3 direction);
Light get_point(glm::vec3 color, glm::vec3 position, float constant, float linear, float quadratic);

#endif
