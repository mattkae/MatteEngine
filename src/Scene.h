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
#include "EditorUI.h"
#include "shaders/Light.shared.cpp"
#include "Box.h"
#include "DebugRender.h"
#include "ModelLoader.h"
#include "UIEventProcessor.h"
#include "Water.h"

struct Scene {
	ModelLoader modelLoader;

	Model models[32];
	Box modelBoundingBoxes[32];
	size_t numModels = 0;
	int selectedModelIndex = -1;
	DebugModel debugModel;

    Light lights[MAX_LIGHTS];
	size_t numLightsUsed = 0;
	ParticleEmitter emitters[32];
	size_t numEmitters = 0;

	Water water;
    Terrain mTerrain;
    Skybox mSkybox;
    bool useDefferredRendering = false;
    DeferredGeometryBuffer mDeferredBuffer;
	EditorUI editorUI;

    bool mUseShadows = false;
    Camera mCamera;
	bool isDying = false;
	UIEventProcessor eventProcessor;

	void initialize();
	void update(double dt);
	void render();
	void free();

	void renderShadows();
	void renderLights();
	void renderGBuffer();
	void renderNonDeferred();
	void renderDebug();
	void renderDirect();
	void renderModels(const ModelUniformMapping& mapping, bool withMaterial = true) const;
};

#endif
