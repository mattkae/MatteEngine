#include "App.h"
#include "App.h"
#include "SceneLoader.h"
#include "GlobalLoaders.h"
#include "Input.h"
#include <GLFW/glfw3.h>

const char* SCENE_PATH = "assets/scenes/big_scene.matte";

void setApplicationDimensions(GLFWwindow* window, int width, int height) {
	if (width == GlobalApp.width && height == GlobalApp.height) {
		return;
	}

	GlobalApp.width = width;
	GlobalApp.height = height;
	GlobalApp.floatWidth = static_cast<GLfloat>(width);
	GlobalApp.floatHeight = static_cast<GLfloat>(height);
	GlobalApp.aspectRatio = GlobalApp.floatWidth / GlobalApp.floatHeight;
	glViewport(0, 0, GlobalApp.width, GlobalApp.height);
}

void Application::initializeWindow() {
	window = glfwCreateWindow(GlobalApp.width, GlobalApp.height, GlobalApp.title, nullptr, nullptr);
	initializeInputSystem(window);
}

void Application::initialize() {
	GlobalLoaders::initialize();
	ShaderUniformMapping::initialize();
	SceneLoader::loadScene(SCENE_PATH, scene);
	editor.initialize(&scene);
}

void Application::update(double deltaTime) {
	if (isKeyJustDown(GLFW_KEY_R, 0)) {
		scene.free();
		editor.free();
		SceneLoader::loadScene(SCENE_PATH, scene);
		editor.initialize(&scene);
		return;
	}

	if (isKeyJustDown(GLFW_KEY_E, 0)) {
		editor.toggle();
	}

	if (isLeftClickDown() && isDefaultFocused()) {
		int modelIdx = castRayToModel();
		if (modelIdx > -1) {
			UIEvent selectionEvent = {UIEventType::SHOW_MODEL, &modelIdx };
			// @TODO: Clicking models in the editor
			//editorUI.ui.eventProcessor.processEvent(selectionEvent);
		}
	}

	scene.update(deltaTime);
	editor.update(deltaTime);
}

int Application::castRayToModel() {
	Ray rayWorld = clickToRay(scene.mCamera);

	GLfloat distanceFromEye = -1;
	int retval = -1;

	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		const Box3D& box = scene.modelBoundingBoxes[mIdx];
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

void Application::render() {
	scene.render();
	editor.render();
}

void Application::free() {
	printf("Freed app");
	deallocateInputSystem();
	scene.free();
	editor.free();
	GlobalLoaders::free();
	ShaderUniformMapping::free();
	freeGlobalShaderRegistry();

	if (window) {
        glfwDestroyWindow(window);
    }
}
