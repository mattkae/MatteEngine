#include "Scene.h"
#include "Camera.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Logger.h"
#include "Ray.h"
#include "SceneLoader.h"
#include "ShaderUniformMapping.h"
#include "App.h"
#include <GLFW/glfw3.h>

void Scene::initialize() {
	WaterParameters waterParameters;
	waterParameters.width = 50;
	waterParameters.height = 50;
	waterParameters.verticesPerUnit = 1.f;
	waterParameters.dudvTexturePath = "assets/water/textures/dudv.jpg";
	water.periodOffsetGradient = PI / 16;
	water.period = PI;
	water.amplitude = 0.3f;
	water.initialize(this, &waterParameters);
}

void Scene::update(double dt) {
	float dtFloat = static_cast<float>(dt);

    mCamera.update(dtFloat);
	water.update(dtFloat);

	for (unsigned int modelIdx = 0; modelIdx < numModels; modelIdx++) {
		models[modelIdx].update(dtFloat);
	}

	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		updateParticleEmitter(emitters[eIdx], dtFloat);
	}


	if (selectedModelIndex > -1) {
		updateDebugModel(debugModel, models[selectedModelIndex].model, mCamera);
	}
}

void Scene::renderShadows() {
	if (!mUseShadows)
        return;

	useShader(ShaderUniformMapping::GlobalShadowShaderMapping.shader);

    for (auto light : lights) {
		light.renderShadows(*this);
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

void Scene::renderModels(const ModelUniformMapping& mapping, bool withMaterial) const {
    mTerrain.render(mapping, withMaterial);

    for (size_t modelIdx = 0; modelIdx < numModels; modelIdx++) {
		models[modelIdx].render(mapping, withMaterial);
    }
}

void Scene::renderDebug() {
	if (selectedModelIndex > -1) {
		renderDebugModel(debugModel, models[selectedModelIndex].model, ShaderUniformMapping::GlobalModelShaderMapping.modelUniformMapping);
	}
}

void Scene::renderDirect(const Camera* camera, Vector4f clipPlane) {
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
	glEnable(GL_CLIP_DISTANCE0);

	mSkybox.render(*camera);

	water.render(lights, numLightsUsed);

	if (!useDefferredRendering) {
		renderNonDeferred();
	}

	useShader(ShaderUniformMapping::GlobalModelShaderMapping.shader);
    camera->render(ShaderUniformMapping::GlobalModelShaderMapping.cameraUniformMapping);

	setShaderVec3(ShaderUniformMapping::GlobalModelShaderMapping.lightUniformMapping.LIGHT_AMBIENT, getVec3(0.3f));
	setShaderInt(ShaderUniformMapping::GlobalModelShaderMapping.lightUniformMapping.LIGHT_NUM_LIGHTS, numLightsUsed);
	setShaderVec4(ShaderUniformMapping::GlobalModelShaderMapping.UNIFORM_CLIP_PLANE, clipPlane);	 

    for (size_t lidx = 0; lidx < numLightsUsed; lidx++) {
		lights[lidx].render(lidx, &ShaderUniformMapping::GlobalModelShaderMapping.lightUniformMapping);
    }

    if (useDefferredRendering) {
		// @TODO: Deferred rendering work
        //mDeferredBuffer.renderToScreen(mSceneShader);
    } else {
        renderModels(ShaderUniformMapping::GlobalModelShaderMapping.modelUniformMapping);
		renderDebug();
		renderNonDeferred();
    }

    glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err, glewGetErrorString(err));
    }
}

void Scene::render() {
	renderShadows();
	renderGBuffer();
	water.renderReflection();
	renderDirect(&mCamera);
}

void Scene::free() {
	for (size_t modelIdx = 0; modelIdx < numModels; modelIdx++) {
        models[modelIdx].free();
    }
	numModels = 0;
	
	for (size_t lidx = 0; lidx < numLightsUsed; lidx++) {
		lights[lidx].free();
    }
	numLightsUsed = 0;

	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		freeParticleEmitter(emitters[eIdx]);
	}
	numEmitters = 0;

	mSkybox.free();
    mTerrain.free();
    mDeferredBuffer.free();
	freeDebug(debugModel);
	water.free();
}
