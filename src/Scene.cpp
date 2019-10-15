#include "Scene.h"
#include "SceneUI.h"
#include "Camera.h"
#include "GlmUtility.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Physics.h"
#include "Logger.h"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Scene::Scene() {
	ui.generate();
    mShadowShader = loadShader("src/shaders/shadows.vert", "src/shaders/shadows.frag");

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error DURING setup: %d\n", err);
    }
}

Scene::Scene(const char *jsonPath) : Scene() { this->loadFromJson(jsonPath); }

Scene::~Scene() {}

void to_json(json &j, const Scene &scene) {
    j = json{{"models", scene.models}, {"lights", scene.lights}, {"spheres", scene.spheres}};
}

void from_json(const json &j, Scene &scene) {
	if (j.count("models") != 0) {
		j.at("models").get_to<std::vector<Model>>(scene.models);
		scene.ui.contexts.push_back(UIContext());
		getModelSelector(scene, scene.ui.contexts.at(0));
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

    if (j.count("useDeferredRendering") == 1) {
        j.at("useDeferredRendering").get_to<bool>(scene.useDefferredRendering);
    }

    if (scene.useDefferredRendering) {
        scene.mDeferredBuffer.generate();
        scene.mSceneShader = loadShader("src/shaders/DefferedModel.vert", "src/shaders/DefferedModel.frag");
		useShader(scene.mSceneShader);
		setShaderInt(scene.mSceneShader, "uPosition", 0);
		setShaderInt(scene.mSceneShader, "uNormal", 1);
		setShaderInt(scene.mSceneShader, "uDiffuse", 2);
		setShaderInt(scene.mSceneShader, "uSpecular", 3);
		setShaderInt(scene.mSceneShader, "uEmissive", 4);
		setShaderInt(scene.mSceneShader, "uMaterialInfo", 5);
    } else {
        scene.mSceneShader = loadShader("src/shaders/model.vert", "src/shaders/model.frag");
		useShader(scene.mSceneShader);
		setShaderInt(scene.mSceneShader, "uDirShadow", 0);
		setShaderInt(scene.mSceneShader, "uMaterial.diffuseTex", 8);
		setShaderInt(scene.mSceneShader, "uMaterial.specularTex", 9);
    }

}

void Scene::loadFromJson(const char *jsonPath) {
    for (auto model : models) {
        freeModel(model);
    }
    models.clear();
    for (auto mSphere : spheres) {
        freeSphere(mSphere);
	}
    spheres.clear();

    for (auto light : lights) {
        light.free();
    }
    lights.clear();

	for (auto emitter : particleEmitters) {
		freeParticleEmitter(emitter);
	}
	particleEmitters.clear();

    mSkybox.free();
    freeTerrain(mTerrain);
    mDeferredBuffer.free();

    std::ifstream sceneFile(jsonPath);

	try {
		json sceneJson = json::parse(sceneFile);
		from_json(sceneJson, *this);
	} catch (std::exception e) {
        std::cerr << "Scene::load_from_json - " << e.what() << "\n";
	}
}

void Scene::update(double dt) {
	float dtFloat = static_cast<float>(dt);

    updateCamera(mCamera, dtFloat);

	for (auto& emitter : particleEmitters) {
		updateParticleEmitter(emitter, dtFloat);
	}

    for (auto& light : lights) {
        light.update(dt);
    }

	ui.update(dt);
}

void Scene::render() const {
    renderShadows();
    renderGBuffer();
    renderScene();
}

void Scene::renderShadows() const {
    if (!mUseShadows)
        return;

	useShader(mShadowShader);

    glCullFace(GL_FRONT);
    for (auto light : lights) {
        light.render_shadows(mShadowShader, *this);
    }
    glCullFace(GL_BACK);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during shadow pass: %d\n", err);
    }
}

void Scene::renderGBuffer() const {
    if (!useDefferredRendering) {
        return;
    }

    mDeferredBuffer.renderToBuffer(mCamera, this);
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during gBuffer pass: %d\n", err);
    }
}

void Scene::renderScene() const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	mSkybox.render(mCamera);

	if (!useDefferredRendering) {
		renderNonDeferred();
	}

	useShader(mSceneShader);
    renderCamera(mCamera, mSceneShader, true);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setShaderVec3(mSceneShader, "uAmbient", glm::vec3(0.3f));
	setShaderInt(mSceneShader, "uNumLights", lights.size());

    for (size_t lidx = 0; lidx < lights.size(); lidx++) {
        lights[lidx].render(mSceneShader, lidx);
    }

    if (useDefferredRendering) {
        mDeferredBuffer.renderToScreen(mSceneShader);
    } else {
        renderModels(mSceneShader);
    }

    glDisable(GL_BLEND);
	renderNonDeferred();

	ui.render();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err, glewGetErrorString(err));
    }
	glDisable(GL_DEPTH_TEST);
}

void Scene::renderNonDeferred() const {
	for (auto emitter : particleEmitters) {
		renderParticleEmitter(emitter, mCamera);
    }
}

void Scene::renderModels(const Shader &shader, bool withMaterial) const {
    renderTerrain(mTerrain, shader, withMaterial);

    for (auto model : models) {
        renderModel(model, shader, withMaterial);
    }

    for (auto sphere : spheres) {
	    renderSphere(sphere, shader, withMaterial);
    }
}
