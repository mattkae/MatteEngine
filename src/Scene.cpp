#include "Scene.h"
#include "Camera.h"
#include "Input.h"
#include "GlmUtility.h"
#include "Physics.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
	// Load shaders
	mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
	mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");
	mSkyboxShader.load("src/shaders/skybox.vert", "src/shaders/skybox.frag");
	mTerrainShader.load("src/shaders/terrain.vert", "src/shaders/terrain.frag", "src/shaders/terrain.geom");
	mParticleShader.load("src/shaders/particle.vert", "src/shaders/particle.frag");

	// Bind uniforms to proper index
	mSceneShader.use();
	mSceneShader.set_uniform_1i("uDirShadows[0]", 0);
	mSceneShader.set_uniform_1i("uDirShadows[1]", 1);
	mSceneShader.set_uniform_1i("uMaterial.diffuseTex", 8);
	mSceneShader.set_uniform_1i("uMaterial.specularTex", 9);

	// Load skybox
	const char* skyboxPaths[6];
	skyboxPaths[0] = "assets/skybox/cloudy/bluecloud_ft.jpg";
	skyboxPaths[1] = "assets/skybox/cloudy/bluecloud_bk.jpg";
	skyboxPaths[2] = "assets/skybox/cloudy/bluecloud_up.jpg";
	skyboxPaths[3] = "assets/skybox/cloudy/bluecloud_dn.jpg";
	skyboxPaths[4] = "assets/skybox/cloudy/bluecloud_rt.jpg";
	skyboxPaths[5] = "assets/skybox/cloudy/bluecloud_lf.jpg";
	initialize_skybox(mSkybox, skyboxPaths);

	// Load terrain
	GenerationParameters params;
	params.size = 512;
	params.granularity = 128;
	params.permSize = 128;
	params.minMaxHeight = 32;
	params.scaleFactor = 0.004;
	params.ampFactor = 0.5;
	params.frequencyFactor = 2.0f;
	params.numOctaves = 64;
	mTerrain = generate_terrain(params);

	// Load particle
	mParticleEmitter.model = glm::scale(mParticleEmitter.model, glm::vec3(0.1));
	mParticleEmitter.numVertices = 3;
	//  mParticleEmitter.drawType = GL_POINTS;
	mParticleEmitter.numParticles = 1000;
	mParticleEmitter.particleDimension = glm::vec3(5.0);
	mParticleEmitter.volumeDimension = glm::vec3(25.0, 25.0, 25.0);
	mParticleEmitter.color = glm::int_rgb_to_float_rgb(glm::vec3(128, 255, 212));
	mParticleEmitter.initialVelocity = glm::vec3(0, 100, 0);
	mParticleEmitter.particleLife = 5;
	mParticleEmitter.spawnRange = glm::vec2(0.1f, 0.5f);
	mParticleEmitter.velocityFunction = [](float t) { return glm::vec3(t * t, t * t * t - t * t, t * t); };
	mParticleEmitter.spawnRange = glm::vec2(0.1f, 0.3f);
	initialize_particle_emitter(mParticleEmitter);
}

Scene::~Scene() {

}

void Scene::set_models(std::vector<Model> models) {
	this->mModels = models;
}

void Scene::set_lights(std::vector<Light> lights) {
	this->mLights = lights;
}

void Scene::update(double dt) {
    move_camera(dt, &mCamera);

    if (Input::getInstance()->is_just_up(GLFW_KEY_F)) {
      mTerrain.wireframeMode = !mTerrain.wireframeMode;
    }

    mCamera.update(dt);
    update_particle_emitter(mParticleEmitter, dt);
}

void Scene::render() {
  render_shadows();
  render_scene();
}

void Scene::render_shadows() {
  if (!mUseShadows) return;
  mShadowShader.use();

  for (auto light : mLights) {
    render_shadows_from_light(light, mShadowShader, *this);
  }
}

void Scene::render_scene() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Skybox
  mSkyboxShader.use();
  render_skybox(mSkybox, mSkyboxShader, mCamera);

  // Terrain
  //mTerrainShader.use();
  //render_terrain(mTerrain, mTerrainShader, mCamera);

  // Particles
  mParticleShader.use();
  render_particle_emitter(mParticleEmitter, mParticleShader, mCamera);

  // Models
  mSceneShader.use();
  mCamera.render(mSceneShader);
  mSceneShader.set_uniform_3f("uAmbient", 0.f, 0.f, 0.f);
  mSceneShader.set_uniform_1i("uNumLights", mLights.size());

  for (int lidx = 0; lidx < mLights.size(); lidx++) {
    render_light(mLights[lidx], mSceneShader, lidx);
  }
  render_models(mSceneShader);
}

void Scene::render_models(const Shader& shader) {
  for (auto model : mModels) {
    model.render(mSceneShader);
  }
}
