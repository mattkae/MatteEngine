#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "Constants.h"
#include "Light.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"
#include "DeferredGeometryBuffer.h"
#include "Terrain.h"
#include "TextRenderer.h"
#include <iostream>
#include <string>
#include <vector>

class Scene {
  public:
    Scene();
    Scene(const char *json);
    ~Scene();

    void loadFromJson(const char *json);
    void render() const;
    void renderModels(const Shader &shader, bool withMaterial = true) const;
    void update(double dt);
    std::vector<Model> models;
    std::vector<Light> lights;
    std::vector<Sphere> spheres;
    std::vector<ParticleEmitter> particleEmitters;
    Terrain mTerrain;
    Skybox mSkybox;
    Shader mSceneShader;
    bool useDefferredRendering = false;
    DeferredGeometryBuffer mDeferredBuffer;
    TextRenderer textRenderer;

  private:
    void renderShadows() const;
    void renderGBuffer() const;
    void renderScene() const;

    bool mUseShadows = true;

    Shader mShadowShader;
    Camera mCamera;
};

#endif
