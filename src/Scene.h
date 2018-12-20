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

using namespace std;

class Camera;

class Scene {
 public:
	Scene();
	~Scene();

	void set_models(std::vector<Model> models);
	void set_lights(std::vector<Light> lights);
	void render();
	void render_models(Shader& shader, bool withMaterial = true);
	void update(double dt);

	vector<Model> get_models() const { return mModels; }
	vector<Light> get_lights() const { return mLights; }
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
  
	vector<Model> mModels;
	vector<Light> mLights;
	Skybox mSkybox;
	Terrain mTerrain;
	ParticleEmitter mParticleEmitter;
	DepthVisualizer mDepthVisualizer;
};

#endif
