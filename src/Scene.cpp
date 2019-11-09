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
#include <GLFW/glfw3.h>

void updateScene(BetterScene& scene, double dt) {
	float dtFloat = static_cast<float>(dt);

	if (scene.shadersToReload.size() > 0) {
		for (Shader shader: scene.shadersToReload) {
			reloadShader(shader);
		}

		scene.shadersToReload.clear();
	}

    updateCamera(scene.mCamera, dtFloat);

	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		updateParticleEmitter(scene.emitters[eIdx], dtFloat);
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
	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		renderParticleEmitter(scene.emitters[eIdx], scene.mCamera);
	}
}

void renderModels(const BetterScene& scene, const Shader &shader, bool withMaterial) {
    renderTerrain(scene.mTerrain, shader, withMaterial);

    for (size_t modelIdx = 0; modelIdx < scene.numModels; modelIdx++) {
        renderModel(scene.models[modelIdx], shader, withMaterial);
    }

    for (size_t sphereIdx = 0; sphereIdx < scene.numSpheres; sphereIdx++) {
	    renderSphere(scene.spheres[sphereIdx], shader, withMaterial);
    }
}

void renderDirect(const BetterScene& scene) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	renderSkybox(scene.mSkybox, scene.mCamera);

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
	setShaderInt(scene.mSceneShader, "uNumLights", scene.numLightsUsed);

    for (size_t lidx = 0; lidx < scene.numLightsUsed; lidx++) {
		renderLight(scene.lights[lidx], scene.mSceneShader, lidx);
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
	for (size_t modelIdx = 0; modelIdx < scene.numModels; modelIdx++) {
        freeModel(scene.models[modelIdx]);
    }
	scene.numModels = 0;

    for (size_t sphereIdx = 0; sphereIdx < scene.numSpheres; sphereIdx++) {
	    freeSphere(scene.spheres[sphereIdx]);
    }
	scene.numSpheres = 0;
	
	for (size_t lidx = 0; lidx < scene.numLightsUsed; lidx++) {
		freeLight(scene.lights[lidx]);
    }
	scene.numLightsUsed = 0;

	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		freeParticleEmitter(scene.emitters[eIdx]);
	}
	scene.numEmitters = 0;

	freeSkybox(scene.mSkybox);
    freeTerrain(scene.mTerrain);
    scene.mDeferredBuffer.free();
	scene.mHotreloadThreader.join();
}