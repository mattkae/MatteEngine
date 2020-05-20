#include "Scene.h"
#include "Camera.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Logger.h"
#include "Ray.h"
#include "SceneLoader.h"
#include <GLFW/glfw3.h>

void Scene::initialize() {
	uiController.scene = this;
	ui.controller = &uiController;
	ui.init();
    ui.showModelSelector(models, numModels);
    isDying = false;
    mShadowShader = loadShader("src/shaders/shadows.vert", "src/shaders/shadows.frag");
}

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

void Scene::update(double dt) {
	float dtFloat = static_cast<float>(dt);
	if (isLeftClickDown() && isDefaultFocused()) {
		int modelIdx = castRayToModel(*this);
		if (modelIdx > -1) {
			uiController.selectModel(modelIdx);
		} else {
			uiController.deselectModel();
		}
	}

	ui.update(dt);

    updateCamera(mCamera, dtFloat);

	for (unsigned int modelIdx = 0; modelIdx < numModels; modelIdx++) {
		models[modelIdx].update(dtFloat);
	}

	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		updateParticleEmitter(emitters[eIdx], dtFloat);
	}


	if (selectedModelIndex > -1) {
		updateDebugModel(debugModel, models[selectedModelIndex].model, mCamera);
	}


	if (isKeyJustDown(GLFW_KEY_R, 0)) {
		free();
		SceneLoader::loadScene("assets/scenes/big_scene.matte", *this);
	}
}

void Scene::renderShadows() {
	if (!mUseShadows)
        return;

	useShader(mShadowShader);

    for (auto light : lights) {
		renderLightShadows(light, mShadowShader, *this);
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during shadow pass: %d\n", err);
    }
}

void Scene::renderGBuffer() {
	if (!useDefferredRendering) {
        return;
    }

    mDeferredBuffer.renderToBuffer(mCamera, *this);
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during gBuffer pass: %d\n", err);
    }
}

void Scene::renderNonDeferred() {
	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		renderParticleEmitter(emitters[eIdx], mCamera);
	}
}

void Scene::renderModels(const Shader &shader, bool withMaterial) const {
    mTerrain.render(shader, withMaterial);

    for (size_t modelIdx = 0; modelIdx < numModels; modelIdx++) {
		if (selectedModelIndex != modelIdx) {
			models[modelIdx].render(shader, withMaterial);
		}
    }
}

void Scene::renderDebug() {
	if (selectedModelIndex > -1) {
		renderDebugModel(debugModel, models[selectedModelIndex].model, mSceneShader);
	}
}

void Scene::renderDirect() {
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	renderSkybox(mSkybox, mCamera);

	if (!useDefferredRendering) {
		renderNonDeferred();
	}

	useShader(mSceneShader);
    renderCamera(mCamera, mSceneShader, true);

	setShaderVec3(mSceneShader, "uAmbient", getVec3(0.3f));
	setShaderInt(mSceneShader, "uNumLights", numLightsUsed);

    for (size_t lidx = 0; lidx < numLightsUsed; lidx++) {
		renderLight(lights[lidx], mSceneShader, lidx);
    }

    if (useDefferredRendering) {
        mDeferredBuffer.renderToScreen(mSceneShader);
    } else {
        renderModels(mSceneShader);
		renderDebug();
		renderNonDeferred();
    }

    glDisable(GL_BLEND);
	ui.render();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err, glewGetErrorString(err));
    }
	glDisable(GL_DEPTH_TEST);
}

void Scene::render() {
	renderShadows();
	renderGBuffer();
	renderDirect();
}

void Scene::free() {
	isDying = true;

	// Models
	for (size_t modelIdx = 0; modelIdx < numModels; modelIdx++) {
        models[modelIdx].free();
    }
	numModels = 0;
	
	// Lights
	for (size_t lidx = 0; lidx < numLightsUsed; lidx++) {
		freeLight(lights[lidx]);
    }
	numLightsUsed = 0;

	// Particle emitters
	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		freeParticleEmitter(emitters[eIdx]);
	}
	numEmitters = 0;

	// UI
	ui.free();

	// Skybox
	freeSkybox(mSkybox);

	// Terrain
    mTerrain.free();

	// Deferred
    mDeferredBuffer.free();

	// Shaders
	if (mShadowShader > 0) {
		glDeleteProgram(mShadowShader);
	}

	// Debug program
	freeDebug(debugModel);
}