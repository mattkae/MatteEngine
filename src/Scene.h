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
#include "shaders/Light.shared.cpp"


struct BetterScene {
	Model models[32];
	size_t numModels;
    Light lights[MAX_LIGHTS];
	size_t numLightsUsed = 0;
	Sphere spheres[32];
	size_t numSpheres;
	ParticleEmitter emitters[32];
	size_t numEmitters;

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

void reloadScene(BetterScene& scene);
void updateScene(BetterScene& scene, double dt);
void renderScene(const BetterScene& scene);
void freeScene(BetterScene& scene);
void renderModels(const BetterScene& scene, const Shader &shader, bool withMaterial = true);

#endif
