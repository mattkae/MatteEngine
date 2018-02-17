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

  // Bind uniforms to proper index
  mSceneShader.Use();
  mSceneShader.SetUniform1i("uDirShadows[0]", 0);
  mSceneShader.SetUniform1i("uDirShadows[1]", 1);

  // Load lights
  Light pLight = get_point(1024, 1024);
  pLight.position = glm::vec3(0.0, 5.0, 0.0);
  pLight.color = glm::vec3(1.0);
  pLight.usesShadows = true;
  mLights.push_back(pLight);
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
    render_shadows_from_light(&light, &mShadowShader, this);
  }
}

void Scene::render_scene() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mSceneShader.Use();
  
  mCamera.render(&mSceneShader);

  mSceneShader.SetUniform3f("uAmbient", 91 / 255.f, 81 / 255.f, 188 / 255.f);
  mSceneShader.SetUniform1i("uNumLights", mLights.size());
    
  for (int lidx = 0; lidx < mLights.size(); lidx++) {
    Light* light = &mLights[lidx];
    render_light(light, &mSceneShader, lidx);
  }

  render_models(&mSceneShader);
}

void Scene::render_models(Shader* shader) {
  for (auto model : mModels) {
    model.render(&mSceneShader);
  }
}
