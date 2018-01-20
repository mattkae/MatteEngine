#include "Scene.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
  mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
  mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");

  mModels.push_back(Model("assets/test.obj")); // Sphere
  Model floor("assets/floor.obj");
  floor.set_model(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -3.0, 0.0)));
  mModels.push_back(floor);
}

Scene::~Scene() {
  
}

void Scene::update(double dt) {
    move_camera(dt, &mCamera);
    mCamera.update(dt);
}

void Scene::render() {
  //  render_shadows();
  render_scene();
}

void Scene::render_shadows() {
  if (!mUseShadows) return;

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);

  mShadowShader.Use();
  
  for (auto light : mLights) {
    if (!light.isOn) continue;
    
    gen_shadow_texture(&mShadowShader, light, mShadowWidth, mShadowHeight);
    
    for (auto model : mModels) {
      glm::mat4 mvp = light.projection * light.view * model.get_model();
      mShadowShader.SetUniformMatrix4fv("u_mvp", 1, GL_FALSE, glm::value_ptr(mvp));
      model.render(&mShadowShader);
    }
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, 800, 600);
}

void Scene::render_scene() {
  mSceneShader.Use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Lights
  mSceneShader.SetUniform3f("u_ambient", 0.1f, 0.1f, 0.1f);
  mSceneShader.SetUniform1i("u_numLights", mLights.size());
  for (int lidx = 0; lidx < mLights.size(); lidx++) {
    Light light = mLights[lidx];
    render_light(&mSceneShader, light, lidx);
  }

  // Models
  for (auto model : mModels) {
    glm::mat4 mvp = mCamera.get_projection() * mCamera.get_view() * model.get_model();
    mSceneShader.SetUniformMatrix4fv("u_mvp", 1, GL_FALSE, glm::value_ptr(mvp));
    model.render(&mSceneShader);
  }
}
