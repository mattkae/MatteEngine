#ifndef LIGHT_H
#define LIGHT_H

#include "gl_includes.h"
#include "ImageDrawer.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <vector>

class Shader;
class Camera;

enum LightType {
  Directional = 0,
  Point,
  Spot,
  Inactive
};

class Light {
 public:
  Light(Camera* camera);
  ~Light();

  void use_shadows(int width, int height);
  void render_shadows(Shader* shader, std::vector<Model>* models);
  void render(Shader* shader, int index);
  glm::mat4 get_projection() { return mProjection; };
  glm::mat4 get_view() { return mView; };

  void set_type(LightType type) { mType = type; update_view(); update_projection(); };
  void toggle_on() { isOn = ! isOn; };
  void set_color(glm::vec3 color) { mColor = color; update_view(); update_projection(); };
  void set_direction_and_up(glm::vec3 direction, glm::vec3 up) { mDirection = direction; mUp = up; update_view(); update_projection(); };
  void set_position(glm::vec3 position) { mPosition = position; update_view(); update_projection(); };
  void set_constant(float constant) { mConstant = constant; update_view(); update_projection(); };
  void set_linear(float linear) { mLinear = linear; update_view(); update_projection(); };
  void set_quadratic(float quadratic) { mQuadratic = quadratic; update_view(); update_projection(); };
  void set_cutoff(float angle) { mCosineCutOff = cos(angle); update_view(); update_projection(); };
  void set_dropoff(float dropoff) { mDropOff = dropoff; update_view(); update_projection(); };
  
  bool isOn = true;
 private:
  ImageDrawer mDrawer;
  Camera* mCamera;
  LightType mType = Directional;
  glm::vec3 mColor = glm::vec3(1.0);
  glm::vec3 mDirection = glm::vec3(0.0, 0.0, -1.0);
  glm::vec3 mUp = glm::vec3(0.0, 1.0, 0.0);
  glm::vec3 mPosition = glm::vec3(0.0);

  // Attenuation
  float mConstant = 1.f;
  float mLinear = 0.f;
  float mQuadratic = 0.f;

  // Spot of light
  float mCosineCutOff = 1.f;
  float mDropOff = 0.f;
  
  glm::mat4 mView = glm::mat4(0.0);
  glm::mat4 mProjection = glm::mat4(0.0);
  bool mUsesShadows = false;
  GLuint mShadowTexture = 0;
  GLuint mDepthFbo = 0;
  int mShadowWidth = 800;
  int mShadowHeight = 600;

  void update_view();
  void update_projection();
};


#endif
