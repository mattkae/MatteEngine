#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "Constants.h"
#include "Light.h"
#include "Model.h"
#include "Particle.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"
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
    void render() const;
    void render_models(const Shader &shader, bool withMaterial = true) const;
    void update(double dt);
    std::vector<Model> mModels;
    std::vector<Light> mLights;
    Terrain mTerrain;
    Skybox mSkybox;
    Sphere sphere;

  private:
    void render_shadows() const;
    void render_scene() const;

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
