#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "GlobalApplicationState.h"
#include "Light.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"
#include "DeferredGeometryBuffer.h"
#include "Terrain.h"
#include "UI/UI.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>

struct BetterScene {
	std::vector<Model> models;
    std::vector<Light> lights;
    std::vector<Sphere> spheres;
    std::vector<ParticleEmitter> particleEmitters;
    Terrain mTerrain;
    Skybox mSkybox;
    Shader mSceneShader;
    bool useDefferredRendering = false;
    DeferredGeometryBuffer mDeferredBuffer;
	UI ui;

	std::thread mHotreloadThreader;
    bool mUseShadows = true;
    Shader mShadowShader;
    BetterCamera mCamera;
	std::vector<Shader> shadersToReload;
	bool isDying = false;
};

void loadSceneFromJson(const char* jsonPath, BetterScene& scene);
void reloadScene(BetterScene& scene);
void updateScene(BetterScene& scene, double dt);
void renderScene(const BetterScene& scene);
void freeScene(BetterScene& scene);
void renderModels(const BetterScene& scene, const Shader &shader, bool withMaterial = true);

#endif
