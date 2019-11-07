#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GlobalApplicationState.h"
#include "Input.h"
#include "Scene.h"
#include "Sphere.h"
#include "SceneLoader.h"
#include "Logger.h"
#include <iostream>
#include <string>

using namespace std;

static GLFWwindow* GlobalWindow = nullptr;

void initialize(int argc, const char* argv[]);
void cleanup();

void glfw_error_callback(int error, const char* message)
{
    cerr << "GLFW error #" << error << ": " << message << endl;
}

int main(int argc, const char* argv[]) {
    initialize(argc, argv);

    BetterScene scene;
	loadScene("assets/scenes/scene.matte", scene);

    glEnable(GL_DEPTH_TEST);
    uint16_t frameCount = 0;
    double frameTimerMs = 0;
    double currentTime = 0, prevTime = glfwGetTime(), deltaTime;
    while (!glfwWindowShouldClose(GlobalWindow)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        glfwPollEvents();

		updateScene(scene, deltaTime);
		renderScene(scene);

        glfwSwapBuffers(GlobalWindow);
        frameCount++;
        frameTimerMs += deltaTime;
        if (frameTimerMs > 1.0) {
            Logger::logInfo(std::to_string(frameCount) + " frames per second");
            frameCount = 0;
            frameTimerMs = 0;
		}
    }

	freeScene(scene);
    cleanup();
    return 0;
}

void initialize(int argc, const char* argv[]) {
    // GLFW
    if (!glfwInit()) {
        cerr << "Failed to initialize glfw." << endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef __APPLE__
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GlobalWindow = glfwCreateWindow(GlobalAppState.width, GlobalAppState.height,
        GlobalAppState.title.c_str(), nullptr, nullptr);
    if (!GlobalWindow) {
        cerr << "Error initializing GLFW window" << endl;
        return;
    }

	initializeInputSystem(GlobalWindow);
    glfwMakeContextCurrent(GlobalWindow);
	glfwSetWindowSizeCallback(GlobalWindow, setApplicationDimensions);

    // GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        cerr << "Unable to initialize GLEW: " << glewGetErrorString(err)
             << endl;
        exit(EXIT_FAILURE);
    }
}

void cleanup() {
    if (GlobalWindow) {
        glfwDestroyWindow(GlobalWindow);
    }

    glfwTerminate();
}
