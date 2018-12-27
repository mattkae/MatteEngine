#ifndef SCENE_H
#define SCENE_H
#include "Model.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Particle.h"
#include "Constants.h"
#include "DepthVisualizer.h"
#include <iostream>
#include <vector>
#include <string>

class Scene {
 public:
	Scene();
	Scene(const char* json);
	~Scene();

	void load_from_json(const char* json);
	void render();
	void render_models(Shader& shader, bool withMaterial = true);
	void update(double dt);
	std::vector<Model> mModels;
	std::vector<Light> mLights;
	Terrain mTerrain;
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
  
	Skybox mSkybox;
	ParticleEmitter mParticleEmitter;
	DepthVisualizer mDepthVisualizer;
};

#endif
