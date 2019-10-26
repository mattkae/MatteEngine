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
#include <GLFW/glfw3.h>

using json = nlohmann::json;

void to_json(json &j, const BetterScene &scene) {
	// @TODO
}

void from_json(const json &j, BetterScene &scene) {
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

void loadSceneFromJson(const char* jsonPath, BetterScene& scene) {
    std::ifstream sceneFile(jsonPath);

	try {
		json sceneJson = json::parse(sceneFile);
		from_json(sceneJson, scene);

		scene.ui.generate();
		scene.isDying = false;
		scene.mShadowShader = loadShader("src/shaders/shadows.vert", "src/shaders/shadows.frag");
		scene.mHotreloadThreader = std::thread(watchForDirectorychanges, std::ref(scene.shadersToReload), std::ref(scene.isDying));

		 GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			printf("Error DURING setup: %d\n", err);
		}

	} catch (std::exception e) {
        std::cerr << "Scene::load_from_json - " << e.what() << "\n";
	}
}

void updateScene(BetterScene& scene, double dt) {
	float dtFloat = static_cast<float>(dt);

	if (scene.shadersToReload.size() > 0) {
		for (Shader shader: scene.shadersToReload) {
			reloadShader(shader);
		}

		scene.shadersToReload.clear();
	}

    updateCamera(scene.mCamera, dtFloat);

	for (auto& emitter : scene.particleEmitters) {
		updateParticleEmitter(emitter, dtFloat);
	}

	scene.ui.update(dt);
}

void renderShadows(const BetterScene& scene) {
	if (!scene.mUseShadows)
        return;

	useShader(scene.mShadowShader);

    glCullFace(GL_FRONT);
    for (auto light : scene.lights) {
		renderLightShadows(light, scene.mShadowShader, scene);
    }
    glCullFace(GL_BACK);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during shadow pass: %d\n", err);
    }
}

void renderGBUffer(const BetterScene& scene) {
	if (!scene.useDefferredRendering) {
        return;
    }

    scene.mDeferredBuffer.renderToBuffer(scene.mCamera, scene);
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during gBuffer pass: %d\n", err);
    }
}

void renderNonDeferred(const BetterScene& scene) {
	for (auto emitter : scene.particleEmitters) {
		renderParticleEmitter(emitter, scene.mCamera);
    }
}

void renderModels(const BetterScene& scene, const Shader &shader, bool withMaterial) {
    renderTerrain(scene.mTerrain, shader, withMaterial);

    for (auto model : scene.models) {
        renderModel(model, shader, withMaterial);
    }

    for (auto sphere : scene.spheres) {
	    renderSphere(sphere, shader, withMaterial);
    }
}


void renderDirect(const BetterScene& scene) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	scene.mSkybox.render(scene.mCamera);

	if (!scene.useDefferredRendering) {
		renderNonDeferred(scene);
	}

	useShader(scene.mSceneShader);
    renderCamera(scene.mCamera, scene.mSceneShader, true);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setShaderVec3(scene.mSceneShader, "uAmbient", glm::vec3(0.3f));
	setShaderInt(scene.mSceneShader, "uNumLights", scene.lights.size());

    for (size_t lidx = 0; lidx < scene.lights.size(); lidx++) {
		renderLight(scene.lights.at(lidx), scene.mSceneShader, lidx);
    }

    if (scene.useDefferredRendering) {
        scene.mDeferredBuffer.renderToScreen(scene.mSceneShader);
    } else {
        renderModels(scene, scene.mSceneShader);
    }

    glDisable(GL_BLEND);
	renderNonDeferred(scene);

	scene.ui.render();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err, glewGetErrorString(err));
    }
	glDisable(GL_DEPTH_TEST);
}

void renderScene(const BetterScene& scene) {
	renderShadows(scene);
	renderGBUffer(scene);
	renderDirect(scene);
}

void freeScene(BetterScene& scene) {
	scene.isDying = true;
	for (auto model : scene.models) {
        freeModel(model);
    }
    scene.models.clear();
    for (auto mSphere : scene.spheres) {
        freeSphere(mSphere);
	}
    scene.spheres.clear();

    for (auto light : scene.lights) {
		freeLight(light);
    }
    scene.lights.clear();

	for (auto emitter : scene.particleEmitters) {
		freeParticleEmitter(emitter);
	}
	scene.particleEmitters.clear();

    scene.mSkybox.free();
    freeTerrain(scene.mTerrain);
    scene.mDeferredBuffer.free();
	scene.mHotreloadThreader.join();
}