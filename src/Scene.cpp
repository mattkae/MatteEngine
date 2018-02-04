#include "Scene.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
  // Load shaders
  mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
  mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");

  // Load models
  mModels.push_back(Model("assets/test.obj"));
  Model floor = Model("assets/floor.obj");
  glm::mat4 floorModel(1.0);
  floorModel = glm::translate(floorModel, glm::vec3(0, -3, 0));
  floor.set_model(floorModel);
  mModels.push_back(floor);

  // Bind lights as default
  /*mSceneShader.Use();
    mSceneShader.SetUniform1i("u_depthTextures[0]", 0);
  mSceneShader.SetUniform1i("u_depthTextures[1]", 1);
  mSceneShader.SetUniform1i("u_depthTextures[2]", 2);
  mSceneShader.SetUniform1i("u_depthTextures[3]", 3);
  mSceneShader.SetUniform1i("u_pointDepthTexture", 4);*/

  // Load lights
  Light light(&mCamera);
  light.set_type(Point);
  light.set_color(glm::vec3(1.0, 1.0, 1.0));
  // light.set_direction_and_up(glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
  light.set_position(glm::vec3(0.0, 5.0, 0.0));
  light.set_constant(1.0);
  light.set_linear(0.0);
  light.set_quadratic(0.0);
  // light.set_cutoff(20.f);
  // light.set_dropoff(24.f);
  light.use_shadows(512, 512);
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
  mCamera.render(&mSceneShader);

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
