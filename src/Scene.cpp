#include "Scene.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
  // Load shaders
  mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
  mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");
  mSkyboxShader.load("src/shaders/skybox.vert", "src/shaders/skybox.frag");
  mTerrainShader.load("src/shaders/terrain.vert", "src/shaders/terrain.frag", "src/shaders/terrain.geom");

  // Load models
  Model model("assets/BaymaxWhiteOBJ/Bigmax_White_OBJ.obj");
  glm::mat4 transform(1.0);
  transform = glm::scale(transform, glm::vec3(0.05));
  transform = glm::translate(transform, glm::vec3(0, 0, 0));
  model.set_model(transform);
  mModels.push_back(model);
  // Bind uniforms to proper index
  mSceneShader.Use();
  mSceneShader.SetUniform1i("uDirShadows[0]", 0);
  mSceneShader.SetUniform1i("uDirShadows[1]", 1);
  mSceneShader.SetUniform1i("uMaterial.diffuseTex", 8);
  mSceneShader.SetUniform1i("uMaterial.specularTex", 9);

  // Load lights
  Light pLight = get_point(512, 512);
  pLight.color = glm::vec3(1.0, 0.0, 0.0);
  pLight.position = glm::vec3(0.0, 5.0, 0.0);
  mLights.push_back(pLight);

  // Load skybox
  const char* skyboxPaths[6];
  skyboxPaths[0] = "assets/skybox/posx.png";
  skyboxPaths[1] = "assets/skybox/negx.png";
  skyboxPaths[2] = "assets/skybox/posy.png";
  skyboxPaths[3] = "assets/skybox/negy.png";
  skyboxPaths[4] = "assets/skybox/posz.png";
  skyboxPaths[5] = "assets/skybox/negz.png";
  initialize_skybox(mSkybox, skyboxPaths);

  // Load terrain
  mTerrain = generate_terrain(500, 50);
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
    render_shadows_from_light(light, mShadowShader, *this);
  }
}

void Scene::render_scene() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mSkyboxShader.Use();
  render_skybox(mSkybox, mSkyboxShader, mCamera);

  mTerrainShader.Use();
  render_terrain(mTerrain, mTerrainShader, mCamera);
  
  mSceneShader.Use();
  mCamera.render(mSceneShader);
  mSceneShader.SetUniform3f("uAmbient", 0.f, 0.f, 0.f);
  mSceneShader.SetUniform1i("uNumLights", mLights.size());

  for (int lidx = 0; lidx < mLights.size(); lidx++) {
    render_light(mLights[lidx], mSceneShader, lidx);
  }
  render_models(mSceneShader);
}

void Scene::render_models(const Shader& shader) {
  for (auto model : mModels) {
    model.render(mSceneShader);
  }
}
