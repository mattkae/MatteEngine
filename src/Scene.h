#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "GlobalApplicationState.h"
#include "Light.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "Shader.h"
#include "Skybox.h"
#include "DeferredGeometryBuffer.h"
#include "Terrain.h"
#include "UI.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "shaders/Light.shared.cpp"
#include "Box.h"
#include "DebugRender.h"

struct BetterScene {
	Model models[32];
	Box modelBoundingBoxes[32];
	size_t numModels = 0;
	int selectedModelIndex = -1;
	DebugModel debugModel;

    Light lights[MAX_LIGHTS];
	size_t numLightsUsed = 0;
	ParticleEmitter emitters[32];
	size_t numEmitters = 0;

    Terrain mTerrain;
    Skybox mSkybox;
    Shader mSceneShader;
    bool useDefferredRendering = false;
    DeferredGeometryBuffer mDeferredBuffer;
	UI ui;

	std::thread mHotreloadThreader;
    bool mUseShadows = false;
    Shader mShadowShader = 0;
    BetterCamera mCamera;
	std::vector<Shader> shadersToReload;
	bool isDying = false;
};

void updateScene(BetterScene& scene, double dt);
void renderScene(const BetterScene& scene);
void freeScene(BetterScene& scene);
void renderModels(const BetterScene& scene, const Shader &shader, bool withMaterial = true);

#endif
