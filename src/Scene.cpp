#include "Scene.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
  mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
  mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");

  Model church("assets/church/Parish Church Model+.obj");
  church.set_model(glm::translate(glm::mat4(1.0), glm::vec3(-25, -50, -50)));
  mModels.push_back(church);

  Light light;
  light.set_type(LightType::Point);
  light.set_position(glm::vec3(0, 2.0, 0));
  light.set_constant(0.88);
  light.set_linear(0.88);
  light.set_quadratic(0.001);
  light.use_shadows( mShadowWidth, mShadowHeight);
  mLights.push_back(light);
}

Scene::~Scene() {
  
}

void Scene::update(double dt) {
    move_camera(dt, &mCamera);
    mCamera.update(dt);
}

void Scene::render() {
  render_shadows();
  render_scene();
}

void Scene::render_shadows() {
  if (!mUseShadows) return;
  mShadowShader.Use();
  
  for (auto light : mLights) {
    light.render_shadows(&mShadowShader, &mModels);
  }
}

void Scene::render_scene() {
  mSceneShader.Use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Lights
  mSceneShader.SetUniform3f("u_ambient", 0.1f, 0.1f, 0.1f);
  mSceneShader.SetUniform1i("u_numLights", mLights.size());
  glm::vec3 eye = mCamera.get_position();
  mSceneShader.SetUniform3f("u_eye", eye.x, eye.y, eye.z);
  for (int lidx = 0; lidx < mLights.size(); lidx++) {
    Light* light = &mLights[lidx];
    light->render(&mSceneShader, lidx);
  }

  // Models
  for (auto model : mModels) {
    glm::mat4 vp = mCamera.get_projection() * mCamera.get_view();
    mSceneShader.SetUniformMatrix4fv("u_vp", 1, GL_FALSE, glm::value_ptr(vp));
    mSceneShader.SetUniformMatrix4fv("u_model", 1, GL_FALSE, glm::value_ptr(model.get_model()));
    
    model.render(&mSceneShader);
  }
}
