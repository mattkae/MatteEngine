#include "Scene.h"

void Scene::Scene() {
  mShadowShader.load("assets/shadows.vert", "assets/shadows.frag");
  mSceneShader.load("assets/model.vert", "assets/model.frag");
}

void Scene::~Scene() {
  
}

void Scene::update(double dt) {
    move_camera(dt, &camera);
    camera.update(dt);
}

void Scene::render() {
  render_shadows();
  render_scene();
}

void Scene::render_shadows() {
  if (!mUseShadows) return;

  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
  glViewport(0, 0, SHADOW_FACTOR * 800, SHADOW_FACTOR * 600);
  glClearDepth(1.0);
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);

  mShadowShader.Use();
  
  for (auto light : mLights) {
    if (light.isOn) continue;
    
    for (auto model :m Models) {
      glm::mat4 mvp = light.projection * light.view * model.get_model();
      mShadowShader.SetUniform4fv("mvp", 1, GL_FALSE, glm::value_ptr(mvp));
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

  mCamera->render(&mSceneShader);

  // Lights
  shader->SetUniform3f("ambient", 0.1f, 0.1f, 0.1f);
  mSceneShader.SetUniform1i("numLights", mLights.size());
  for (auto light : mLights) {
    light.render(&mSceneShader);
  }

  // Models
  for (auto model : mModels) {
    model.render(&mSceneShader);
  }
}
