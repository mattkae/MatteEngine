#include "Scene.h"
#include "SceneUI.h"
#include "Camera.h"
#include "GlmUtility.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Physics.h"
#include "Logger.h"
#include "Ray.h"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

int castRayToModel(BetterScene& scene) {
	Vector4f rayWorld = clickToRay(scene.mCamera);

	GLfloat distanceFromEye = -1;
	int retval = -1;

	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		const Box& box = scene.modelBoundingBoxes[mIdx];
		const Model& model = scene.models[mIdx];

		if (isBoxInRayPath(box, model.model, rayWorld, scene.mCamera)) {
			GLfloat nextDistanceFromEye = getDistanceFromCamera(box, scene.mCamera, model.model);
			if (distanceFromEye < 0 || nextDistanceFromEye < distanceFromEye) {
				distanceFromEye = nextDistanceFromEye;
				retval = mIdx;
			}
		}
	}
	return retval;
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

	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		updateParticleEmitter(scene.emitters[eIdx], dtFloat);
	}

	if (isLeftClickDown() && isDefaultFocused()) {
		int modelIdx = castRayToModel(scene);
		if (modelIdx > -1) {
			openModelPanel(scene.ui, modelIdx);
			scene.selectedModelIndex = modelIdx;
			scene.debugModel.debugBox = scene.modelBoundingBoxes[modelIdx];
		}
	}

	if (scene.selectedModelIndex > -1) {
		updateDebugModel(scene.debugModel, scene.models[scene.selectedModelIndex].model, scene.mCamera);
	}

	updateUI(scene.ui, dt);

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
		if (scene.selectedModelIndex != modelIdx)
			renderModel(scene.models[modelIdx], shader, withMaterial);
    }
}

void renderDebug(const BetterScene& scene) {
	if (scene.selectedModelIndex > -1) {
		renderDebugModel(scene.debugModel, scene.models[scene.selectedModelIndex].model, scene.mSceneShader);
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
	renderDebug(scene);

	renderUI(scene.ui);


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
	
	for (size_t lidx = 0; lidx < scene.numLightsUsed; lidx++) {
		freeLight(scene.lights[lidx]);
    }
	scene.numLightsUsed = 0;

	for (size_t eIdx = 0; eIdx < scene.numEmitters; eIdx++) {
		freeParticleEmitter(scene.emitters[eIdx]);
	}
	scene.numEmitters = 0;

	freeUI(scene.ui);
	freeSkybox(scene.mSkybox);
    freeTerrain(scene.mTerrain);
    scene.mDeferredBuffer.free();
	scene.mHotreloadThreader.join();
}