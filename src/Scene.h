#ifndef SCENE_H
#define SCENE_H
#include "Model.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Particle.h"
//#include "Smoke.h"
#include "Constants.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Camera;

struct ModelInfo {
	std::string path;
	glm::mat4 transform;
};

struct SceneInfo {
	std::vector<ModelInfo> models;
};

class Scene {
 public:
  Scene(const SceneInfo& info);
  ~Scene();
  void render();
  void render_models(const Shader& shader);
  void update(double dt);
 private:
  void render_shadows();
  void render_scene();
  
  bool mUseShadows = true;

  Shader mShadowShader;
  Shader mSceneShader;
  Shader mSkyboxShader;
  Shader mTerrainShader;
  Shader mParticleShader;
  Camera mCamera;
  
  vector<Model> mModels;
  vector<Light> mLights;
  Skybox mSkybox;
  Terrain mTerrain;
  ParticleEmitter mParticleEmitter;
};

#endif
