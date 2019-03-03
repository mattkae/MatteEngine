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

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error DURING setup: %d\n", err);
    }
}

Scene::Scene(const char *jsonPath) : Scene() { this->load_from_json(jsonPath); }

Scene::~Scene() {}

void to_json(json &j, const Scene &scene) {
    j = json{{"models", scene.models}, {"lights", scene.lights}, {"spheres", scene.spheres}};
}

void from_json(const json &j, Scene &scene) {
	if (j.count("models") != 0) {
		j.at("models").get_to<std::vector<Model>>(scene.models);
	}
	if (j.count("lights") != 0) {
		j.at("lights").get_to<std::vector<Light>>(scene.lights);
	}
	if (j.count("spheres") != 0) {
		j.at("spheres").get_to<std::vector<Sphere>>(scene.spheres);
	}
	if (j.count("particleEmitters") != 0) {
		j.at("particleEmitters").get_to<std::vector<ParticleEmitter>>(scene.particleEmitters);
	}
    if (j.count("terrain") == 1) {
        j.at("terrain").get_to<Terrain>(scene.mTerrain);
    }
    if (j.count("skybox") == 1) {
        j.at("skybox").get_to<Skybox>(scene.mSkybox);
    }
}

void Scene::load_from_json(const char *jsonPath) {
    for (auto model : models) {
        model.free();
    }
    models.clear();
    for (auto mSphere : spheres) {
        mSphere.free();
	}
    spheres.clear();

    for (auto light : lights) {
        light.free();
    }
    lights.clear();

	for (auto emitter : particleEmitters) {
		emitter.free();
	}
	particleEmitters.clear();

    mSkybox.free();
    mTerrain.free();

    std::ifstream sceneFile(jsonPath);

	try {
		json sceneJson = json::parse(sceneFile);
		from_json(sceneJson, *this);
	} catch (std::exception e) {
        std::cerr << "Scene::load_from_json - " << e.what() << "\n";
	}
}

void Scene::update(double dt) {
    move_camera(dt, &mCamera);

    if (Input::getInstance()->is_just_up(GLFW_KEY_F)) {
        mTerrain.wireframeMode = !mTerrain.wireframeMode;
    }

    mCamera.update(dt);
    
	for (auto& emitter : particleEmitters) {
		emitter.update(dt);
	}
}

void Scene::render() const {
    render_shadows();
    render_scene();
}

void Scene::render_shadows() const {
    if (!mUseShadows)
        return;

    mShadowShader.use();

    for (auto light : lights) {
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
	for (auto emitter : particleEmitters) {
		emitter.render(mParticleShader, mCamera);
	}

    // Models
    mSceneShader.use();
    mCamera.render(mSceneShader);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    mSceneShader.set_uniform_3f("uAmbient", 0.f, 0.f, 0.f);
    mSceneShader.set_uniform_1i("uNumLights", lights.size());

    for (int lidx = 0; lidx < lights.size(); lidx++) {
        lights[lidx].render(mSceneShader, lidx);
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
    for (auto model : models) {
        model.render(shader, withMaterial);
    }

    for (auto sphere : spheres) {
	sphere.render(shader, withMaterial);
    }
}
