#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "Skybox.h"
#include "Terrain.h"
#include "shaders/Light.shared.cpp"
#include "Box.h"
#include "DebugRender.h"
#include "ModelLoader.h"
#include "Water.h"
#include "GradientSky.h"
#include "SystemEngine.h"
#include "FrameBuffer.h"
#include "Quad.h"
#include <climits>

struct Scene {
	SystemEngine systemEngine;

	ParticleEmitter emitters[32];
	size_t numEmitters = 0;

	Water water;
    Terrain mTerrain;
	GradientSky mGradientSky;

    bool mUseShadows = true;
    Camera mCamera;
	TextureFrameBuffer postProcessBuffer;
	Quad postProcessQuad;

	void initialize();
	void update(double dt);
	void render();
	void free();

	void renderParticles();
	void renderDirect(const Camera* camera, Vector4f clipPlane = { 0, 1, 0, static_cast<float>(INT_MAX) });
	void renderModels(const ModelUniformMapping& mapping, bool withMaterial = true) const;
};

#endif
