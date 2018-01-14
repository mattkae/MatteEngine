#ifndef LIGHTS_H
#define LIGHTS_H
#include <vector>
#include <glm/glm.hpp>

class Shader;

const unsigned int Directional = 0x00000001u;
const unsigned int Point       = 0x00000002u;
const unsigned int Spot        = 0x00000004u;
const int MAX_LIGHTS = 16;

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
};

class LightSystem {
 public:
  LightSystem();
  int add_directional(glm::vec3 direction, glm::vec3 color);
  int add_point(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);
  int add_spot(glm::vec3 direction, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic, float cosineCutOff, float dropOff);
  void set_ambient(glm::vec3 ambient);
  void render(Shader* shader);

 private:
  glm::vec3 mAmbient = glm::vec3(1.0);
  int mNumLights = 0;
  Light mLights[MAX_LIGHTS];
};

#endif