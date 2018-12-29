#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "Constants.h"
#include "Light.h"
#include "Model.h"
#include "Particle.h"
#include "Shader.h"
#include "Skybox.h"
#include "Terrain.h"
#include <iostream>
#include <string>
#include <vector>

class Scene {
  public:
    Scene();
    Scene(const char *json);
    ~Scene();

    void load_from_json(const char *json);
    void render();
    void render_models(Shader &shader, bool withMaterial = true);
    void update(double dt);
    std::vector<Model> mModels;
    std::vector<Light> mLights;
    Terrain mTerrain;
    Skybox mSkybox;

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

    ParticleEmitter mParticleEmitter;
};

#endif
