#ifndef SCENE_H
#define SCENE_H
#include "Model.h"
#include "Light.h"
#include "Shader.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Scene {
 public:
  Scene();
  ~Scene();
  void render();
  void update(double dt);
  void load(string path);
 private:
  bool mUseShadows = true;

  Shader mShadowShader;
  Shader mSceneShader;
  
  vector<Model> mModels;
  vector<Light> mLights;
};

#endif
