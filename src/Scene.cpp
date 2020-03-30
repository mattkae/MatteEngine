#include "Scene.h"
#include "SceneUI.h"
#include "Camera.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Logger.h"
#include "Ray.h"
#include "SceneLoader.h"
#include <fstream>
#include <GLFW/glfw3.h>

int castRayToModel(Scene& scene) {
	Ray rayWorld = clickToRay(scene.mCamera);

	GLfloat distanceFromEye = -1;
	int retval = -1;

	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		const Box& box = scene.modelBoundingBoxes[mIdx];
		const Model& model = scene.models[mIdx];

		if (isBoxInRayPath(box, model.model, rayWorld)) {
			GLfloat nextDistanceFromEye = getDistanceFromCamera(box, scene.mCamera, model.model);
			if (distanceFromEye < 0 || nextDistanceFromEye < distanceFromEye) {
				distanceFromEye = nextDistanceFromEye;
				retval = mIdx;
			}
		}
	}
	return retval;
}

void updateScene(Scene& scene, double dt) {
	float dtFloat = static_cast<float>(dt);

	if (scene.shadersToReload.size() > 0) {
		for (Shader shader: scene.shadersToReload) {
			reloadShader(shader);
		}

		scene.shadersToReload.clear();
	}

    updateCamera(scene.mCamera, dtFloat);

	for (unsigned int modelIdx = 0; modelIdx < scene.numModels; modelIdx++) {
		scene.models[modelIdx].update(dtFloat);
	}

	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		updateParticleEmitter(scene.emitters[eIdx], dtFloat);
	}

	if (isLeftClickDown() && isDefaultFocused()) {
		int modelIdx = castRayToModel(scene);
		if (modelIdx > -1) {
			openModelPanel(scene.ui, modelIdx);
			scene.selectedModelIndex = modelIdx;
			scene.debugModel.debugBox = scene.modelBoundingBoxes[modelIdx];
		} else {
			scene.selectedModelIndex = -1;
		}
	}

	if (scene.selectedModelIndex > -1) {
		updateDebugModel(scene.debugModel, scene.models[scene.selectedModelIndex].model, scene.mCamera);
	}

	updateUI(scene.ui, dt);

	if (isKeyJustDown(GLFW_KEY_R, 0)) {
		freeScene(scene);
		SceneLoader::loadScene("assets/scenes/big_scene.matte", scene);
	}
}

void renderShadows(const Scene& scene) {
	if (!scene.mUseShadows)
        return;

	useShader(scene.mShadowShader);

    for (auto light : scene.lights) {
		renderLightShadows(light, scene.mShadowShader, scene);
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during shadow pass: %d\n", err);
    }
}

void renderGBuffer(const Scene& scene) {
	if (!scene.useDefferredRendering) {
        return;
    }

    scene.mDeferredBuffer.renderToBuffer(scene.mCamera, scene);
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during gBuffer pass: %d\n", err);
    }
}

void renderNonDeferred(const Scene& scene) {
	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		renderParticleEmitter(scene.emitters[eIdx], scene.mCamera);
	}
}

void renderModels(const Scene& scene, const Shader &shader, bool withMaterial) {
    renderTerrain(scene.mTerrain, shader, withMaterial);

    for (size_t modelIdx = 0; modelIdx < scene.numModels; modelIdx++) {
		if (scene.selectedModelIndex != modelIdx)
			scene.models[modelIdx].render(shader, withMaterial);
    }
}

void renderDebug(const Scene& scene) {
	if (scene.selectedModelIndex > -1) {
		renderDebugModel(scene.debugModel, scene.models[scene.selectedModelIndex].model, scene.mSceneShader);
	}
}

void renderDirect(const Scene& scene) {
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	renderSkybox(scene.mSkybox, scene.mCamera);

	if (!scene.useDefferredRendering) {
		renderNonDeferred(scene);
	}

	useShader(scene.mSceneShader);
    renderCamera(scene.mCamera, scene.mSceneShader, true);

	setShaderVec3(scene.mSceneShader, "uAmbient", getVec3(0.5f));
	setShaderInt(scene.mSceneShader, "uNumLights", scene.numLightsUsed);

    for (size_t lidx = 0; lidx < scene.numLightsUsed; lidx++) {
		renderLight(scene.lights[lidx], scene.mSceneShader, lidx);
    }

    if (scene.useDefferredRendering) {
        scene.mDeferredBuffer.renderToScreen(scene.mSceneShader);
    } else {
        renderModels(scene, scene.mSceneShader);
		renderDebug(scene);
		renderNonDeferred(scene);
    }

    glDisable(GL_BLEND);

	renderUI(scene.ui);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err, glewGetErrorString(err));
    }
	glDisable(GL_DEPTH_TEST);
}

void renderScene(const Scene& scene) {
	renderShadows(scene);
	renderGBuffer(scene);
	renderDirect(scene);
}

void freeScene(Scene& scene) {
	scene.isDying = true;

	// Models
	for (size_t modelIdx = 0; modelIdx < scene.numModels; modelIdx++) {
        scene.models[modelIdx].free();
    }
	scene.numModels = 0;
	
	// Lights
	for (size_t lidx = 0; lidx < scene.numLightsUsed; lidx++) {
		freeLight(scene.lights[lidx]);
    }
	scene.numLightsUsed = 0;

	// Particle emitters
	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		freeParticleEmitter(scene.emitters[eIdx]);
	}
	scene.numEmitters = 0;

	// UI
	freeUI(scene.ui);

	// Skybox
	freeSkybox(scene.mSkybox);

	// Terrain
    freeTerrain(scene.mTerrain);

	// Deferred
    scene.mDeferredBuffer.free();

	// Threads
	scene.mHotreloadThreader.join();

	// Shaders
	if (scene.mShadowShader > 0) {
		glDeleteProgram(scene.mShadowShader);
	}

	// Debug program
	freeDebug(scene.debugModel);
}