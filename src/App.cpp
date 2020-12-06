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

	scene.update(deltaTime);
	editor.update(deltaTime);
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