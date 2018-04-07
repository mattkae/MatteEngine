#ifndef SCENE_H
#define SCENE_H
#include "Model.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Particle.h"
#include "Constants.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Camera;

class Scene {
 public:
  Scene();
  ~Scene();
  void render();
  void render_models(const Shader& shader);
  void update(double dt);
  void load(string path);
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
