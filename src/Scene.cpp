#include "Scene.h"
#include "Camera.h"
#include "ImageUtil.h"
#include "Input.h"
#include "Logger.h"
#include "Ray.h"
#include "SceneLoader.h"
#include "ShaderUniformMapping.h"
#include "App.h"
#include "Quad.h"
#include <GLFW/glfw3.h>

void Scene::initialize() {
	/*
	WaterParameters waterParameters;
	waterParameters.width = 1000;
	waterParameters.height = 1000;
	waterParameters.verticesPerUnit = 1.f;
	waterParameters.dudvTexturePath = "assets/water/textures/dudv.jpg";
	waterParameters.normalMapPath = "assets/water/textures/normal.png";
	water.periodOffsetGradient = PI / 16;
	water.dudvSpeed = 0.1f;
	water.period = PI;
	water.amplitude = 0.f;
	water.initialize(this, &waterParameters);
	waterParameters.dudvTexturePath.free();
	waterParameters.normalMapPath.free();
	*/

	mGradientSky.startColor = { 0.9f, 0.9f, 0.9f, 1.f };
	mGradientSky.endColor = { 0.1f, 0.2f, 0.5f, 1.0f };
	mGradientSky.mixStartEnd = { 0.f, 200.f };
	mGradientSky.initialize(750.f, 5.f, 5.f);

	systemEngine.initialize();

	postProcessBuffer = createFloatingPointFrameBuffer(GlobalApp.width, GlobalApp.height);
	postProcessQuad.generate();
}

void Scene::update(double dt) {
	float dtFloat = static_cast<float>(dt);

    mCamera.update(dtFloat);
	water.update(dtFloat);

	systemEngine.update(dtFloat);

	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		updateParticleEmitter(emitters[eIdx], dtFloat);
	}
}

void Scene::renderParticles() {
	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		renderParticleEmitter(emitters[eIdx], mCamera);
	}
}

void Scene::renderModels(const ModelUniformMapping& mapping, bool withMaterial) const {
    mTerrain.render(mapping, withMaterial);
	systemEngine.mRenderSystem.render(mapping, withMaterial);
}

void Scene::renderDirect(const Camera* camera, Vector4f clipPlane) {
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	// @TODO: Turn back on transparent objects, but they never really even worked anyway
    //glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_FRAMEBUFFER_SRGB); 
	mGradientSky.render(camera);

	// water.render(lights, numLightsUsed);

	renderParticles();

	useShader(ShaderUniformMapping::GlobalModelShaderMapping.shader);
    camera->render(ShaderUniformMapping::GlobalModelShaderMapping.cameraUniformMapping);

	setShaderVec3(ShaderUniformMapping::GlobalModelShaderMapping.lightUniformMapping.LIGHT_AMBIENT, getVec3(0.3f));
	setShaderVec4(ShaderUniformMapping::GlobalModelShaderMapping.UNIFORM_CLIP_PLANE, clipPlane);	 

	systemEngine.mLightSystem.render(&ShaderUniformMapping::GlobalModelShaderMapping.lightUniformMapping);
    renderModels(ShaderUniformMapping::GlobalModelShaderMapping.modelUniformMapping);
	renderParticles();

    glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during scene pass: %i, %s\n", err, glewGetErrorString(err));
    }
}

void Scene::render() {
	if (mUseShadows) {
		systemEngine.mLightSystem.renderShadows(this);
	}

	water.renderReflection();

	bool postProcess = true;

	// Render out entire scene to the Framebuffer first
	if (postProcess) {
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessBuffer.fbo);
	}
	renderDirect(&mCamera);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Render the framebuffer texture to the scene
	if (postProcess) {
		useShader(ShaderUniformMapping::GlobalPostProcessorShaderMapping.shader);
		setShaderInt(ShaderUniformMapping::GlobalPostProcessorShaderMapping.UNIFORM_SCENE_BUFFER, 0);
		setShaderFloat(ShaderUniformMapping::GlobalPostProcessorShaderMapping.UNIFORM_EXPOSURE, 0.1f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, postProcessBuffer.texture);
		postProcessQuad.render();
	}
}

void Scene::free() {
	systemEngine.free();

	for (size_t eIdx = 0; eIdx < numEmitters; eIdx++) {
		freeParticleEmitter(emitters[eIdx]);
	}
	numEmitters = 0;

    mTerrain.free();
	water.free();
	FrameBuffer::freeFrameBuffer(&postProcessBuffer);
}
