#ifndef LIGHTS_H
#define LIGHTS_H
#include "gl_includes.h"
#include <vector>
#include <glm/glm.hpp>

class Shader;
class Model;

const unsigned int Directional = 0x00000001u;
const unsigned int Point       = 0x00000002u;
const unsigned int Spot        = 0x00000004u;
const unsigned int Inactive    = 0x00000008u;
const int MAX_LIGHTS = 4;

struct Light {
  glm::vec3 direction = glm::vec3(0.0, 0.0, -1.0);
  glm::vec3 position = glm::vec3(0.0);
  glm::vec3 color = glm::vec3(1.0);
  float constant = 1.f;
  float linear = 0.f;
  float quadratic = 0.f;
  float cosineCutOff = 0.99f;
  float dropOff = 0.f;
  unsigned int type = Directional;
  bool isOn = true;
  glm::mat4 view;
  glm::mat4 projection;
};

class LightSystem {
 public:
  LightSystem();
  int add_directional(glm::vec3 direction, glm::vec3 color);
  int add_point(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);
  int add_spot(glm::vec3 direction, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic, float cosineCutOff, float dropOff);
  void set_ambient(glm::vec3 ambient);
  void render_shadows(Shader* shader, Model* model, Model* floor);
  void render(Shader* shader);
  void toggle(int id);

 private:
  // Lights
  glm::vec3 mAmbient = glm::vec3(1.0);
  int mNumLights = 0;
  Light mLights[MAX_LIGHTS];

  // Shadow Variables
  GLuint mDepthTexture = 0;
  GLuint mDepthFbo = 0;

  // ------------DEBUG----------
  Shader* texShader;
};

#endif
