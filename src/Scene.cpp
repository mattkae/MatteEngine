#include "Scene.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
  mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
  mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");

  Model suit("assets/suit/nanosuit.obj");
  glm::mat4 model(1.0);
  model = glm::scale(model, glm::vec3(0.2));
  model = glm::translate(model, glm::vec3(0, 0, 0));
  suit.set_model(model);
  mModels.push_back(suit);
  //mModels.push_back(Model("assets/test.obj"));
  Model floor = Model("assets/floor.obj");
  glm::mat4 floorModel(1.0);
  floorModel = glm::translate(floorModel, glm::vec3(0, -3, 0));
  floor.set_model(floorModel);
  mModels.push_back(floor);

  Light light;
  light.set_color(glm::vec3(1.0, 1.0, 1.0));
  light.set_direction_and_up(glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
  light.use_shadows(mShadowWidth, mShadowHeight);
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
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mSceneShader.Use();
  
  // Camera
  glm::mat4 vp = mCamera.get_projection() * mCamera.get_view();
  mSceneShader.SetUniformMatrix4fv("u_vp", 1, GL_FALSE, glm::value_ptr(vp));
  glm::vec3 eye = mCamera.get_position();
  mSceneShader.SetUniform3f("u_eye", eye.x, eye.y, eye.z);

  // Lights
  mSceneShader.SetUniform3f("u_ambient", 91 / 255.f, 81 / 255.f, 188 / 255.f);
  mSceneShader.SetUniform1i("u_numLights", mLights.size());
    
  for (int lidx = 0; lidx < mLights.size(); lidx++) {
    Light* light = &mLights[lidx];
    light->render(&mSceneShader, lidx);
  }

  // Models
  for (auto model : mModels) {
    mSceneShader.SetUniformMatrix4fv("u_model", 1, GL_FALSE, glm::value_ptr(model.get_model()));
    model.render(&mSceneShader);
  }
}
