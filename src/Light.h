#ifndef LIGHT_H
#define LIGHT_H
#define GLM_ENABLE_EXPERIMENTAL

#include "gl_includes.h"
#include <glm/glm.hpp>

class Shader;
class Scene;

enum LightType {
  Directional = 0,
  Point,
  Spot,
  Inactive
};

struct Light {
  LightType type;
  glm::vec3 color = glm::vec3(1.0);
  glm::vec3 direction = glm::vec3(1.0);
  glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
  glm::vec3 position = glm::vec3(0.0);

  float constant = 1.0f;
  float linear = 0.0f;
  float quadratic = 0.f;

  float cosineCutOff = 1.0f;
  float dropOff = 1.0f;

  bool usesShadows = false;
  GLuint shadowTexture = 0;
  GLuint depthFbo = 0;
  int shadowWidth = 800;
  int shadowHeight = 600;

  bool isOn = true;

  glm::mat4 projection;
};

Light get_point(int width, int height);
Light get_spot(int width, int height);
Light get_directional(int width, int height);
void render_shadows_from_light(const Light& light, Shader& shader, Scene& scene);
void render_light(const Light& light, Shader& shader, int index);
#endif
