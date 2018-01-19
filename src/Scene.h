#ifndef SCENE_H
#define SCENE_H
#include "Model.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
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
  void update(double dt);
  void load(string path);
 private:
  void render_shadows();
  void render_scene();
  
  bool mUseShadows = true;
  int mShadowWidth = 800 * 2;
  int mShadowHeight = 600 * 2;

  Shader mShadowShader;
  Shader mSceneShader;
  Camera mCamera;
  
  vector<Model> mModels;
  vector<Light> mLights;
};

#endif
