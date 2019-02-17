#include "Scene.h"
#include "Camera.h"
#include "GlmUtility.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Physics.h"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Scene::Scene() {
    // Load shaders
    mSceneShader.load("src/shaders/model.vert", "src/shaders/model.frag");
    mShadowShader.load("src/shaders/shadows.vert", "src/shaders/shadows.frag");
    mSkyboxShader.load("src/shaders/skybox.vert", "src/shaders/skybox.frag");
    mTerrainShader.load("src/shaders/terrain.vert", "src/shaders/terrain.frag",
                        "src/shaders/terrain.geom");
    mParticleShader.load("src/shaders/particle.vert",
                         "src/shaders/particle.frag");

    // Bind uniforms to proper index
    mSceneShader.use();
    mSceneShader.set_uniform_1i("uDirShadow", 0);
    mSceneShader.set_uniform_1i("uMaterial.diffuseTex", 8);
    mSceneShader.set_uniform_1i("uMaterial.specularTex", 9);

    // Load particle
    mParticleEmitter.model = glm::scale(mParticleEmitter.model, glm::vec3(0.1));
    mParticleEmitter.numVertices = 3;
    //  mParticleEmitter.drawType = GL_POINTS;
    mParticleEmitter.numParticles = 1000;
    mParticleEmitter.particleDimension = glm::vec3(5.0);
    mParticleEmitter.volumeDimension = glm::vec3(25.0, 25.0, 25.0);
    mParticleEmitter.color =
        glm::int_rgb_to_float_rgb(glm::vec3(128, 255, 212));
    mParticleEmitter.initialVelocity = glm::vec3(0, 100, 0);
    mParticleEmitter.particleLife = 5;
    mParticleEmitter.spawnRange = glm::vec2(0.1f, 0.5f);
    mParticleEmitter.velocityFunction = [](float t) {
        return glm::vec3(t * t, t * t * t - t * t, t * t);
    };
    mParticleEmitter.spawnRange = glm::vec2(0.1f, 0.3f);
    initialize_particle_emitter(mParticleEmitter);
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error DURING setup: %d\n", err);
    }
}

Scene::Scene(const char *jsonPath) : Scene() { this->load_from_json(jsonPath); }

Scene::~Scene() {}

void to_json(json &j, const Scene &scene) {
    j = json{{"models", scene.mModels}, {"lights", scene.mLights}, {"spheres", scene.spheres}};
}

void from_json(const json &j, Scene &scene) {
    j.at("models").get_to<std::vector<Model>>(scene.mModels);
    j.at("lights").get_to<std::vector<Light>>(scene.mLights);
    j.at("spheres").get_to<std::vector<Sphere>>(scene.spheres);
    if (j.count("terrain") == 1) {
        j.at("terrain").get_to<Terrain>(scene.mTerrain);
    }
    if (j.count("skybox") == 1) {
        j.at("skybox").get_to<Skybox>(scene.mSkybox);
    }
}

void Scene::load_from_json(const char *jsonPath) {
    for (auto model : mModels) {
        model.free_resources();
    }
    mModels.clear();

    for (auto light : mLights) {
        light.free();
    }
    mLights.clear();

    mSkybox.free();
    mTerrain.free();

    std::ifstream sceneFile(jsonPath);
    json sceneJson = json::parse(sceneFile);
    from_json(sceneJson, *this);
}

void Scene::update(double dt) {
    move_camera(dt, &mCamera);

    if (Input::getInstance()->is_just_up(GLFW_KEY_F)) {
        mTerrain.wireframeMode = !mTerrain.wireframeMode;
    }

    mCamera.update(dt);
    update_particle_emitter(mParticleEmitter, dt);
}

void Scene::render() const {
    render_shadows();
    render_scene();
}

void Scene::render_shadows() const {
    if (!mUseShadows)
        return;

    mShadowShader.use();

    for (auto light : mLights) {
        light.render_shadows(mShadowShader, *this);
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error DURING shadow pass: %d\n", err);
    }
}

void Scene::render_scene() const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Skybox
    if (mSkybox.isInited) {
        mSkyboxShader.use();
        mSkybox.render(mSkyboxShader, mCamera);
    }

    // Terrain
    if (mTerrain.isInited()) {
        mTerrainShader.use();
        mTerrain.render(mTerrainShader, mCamera);
    }

    // Particles
    mParticleShader.use();
    render_particle_emitter(mParticleEmitter, mParticleShader, mCamera);

    // Models
    mSceneShader.use();
    mCamera.render(mSceneShader);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    mSceneShader.set_uniform_3f("uAmbient", 0.f, 0.f, 0.f);
    mSceneShader.set_uniform_1i("uNumLights", mLights.size());

    for (int lidx = 0; lidx < mLights.size(); lidx++) {
        mLights[lidx].render(mSceneShader, lidx);
    }

    render_models(mSceneShader);
    glDisable(GL_BLEND);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err,
               glewGetErrorString(err));
    }
}

void Scene::render_models(const Shader &shader, bool withMaterial) const {
    for (auto model : mModels) {
        model.render(shader, withMaterial);
    }

    for (auto sphere : spheres) {
	sphere.render(shader, withMaterial);
    }
}
