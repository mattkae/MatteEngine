#ifndef SCENE_H
#define SCENE_H
#include "Model.h"
#include "Light.h"
#include <iostream>
#include <vector>

using namespace std;

class Scene {
 public:
  Scene();
  ~Scene();
  void render();
 private:
  vector<Model> mModels;
  vector<Light> mLights;
};

#endif
