#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "Skybox.h"
#include "DeferredGeometryBuffer.h"
#include "Terrain.h"
#include "shaders/Light.shared.cpp"
#include "Box.h"
#include "DebugRender.h"
#include "ModelLoader.h"
#include "Water.h"
#include <climits>

struct Scene {
	ModelLoader modelLoader;

	Model models[32];
	Box3D modelBoundingBoxes[32];
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

    bool mUseShadows = false;
    Camera mCamera;

	void initialize();
	void update(double dt);
	void render();
	void free();

	void renderShadows();
	void renderGBuffer();
	void renderNonDeferred();
	void renderDebug();
	void renderDirect(const Camera* camera, Vector4f clipPlane = { 0, 1, 0, static_cast<float>(INT_MAX) });
	void renderModels(const ModelUniformMapping& mapping, bool withMaterial = true) const;
};

#endif
