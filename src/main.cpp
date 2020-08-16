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

void initialize(int argc, const char* argv[]);
void cleanup();

void glfw_error_callback(int error, const char* message)
{
    cerr << "GLFW error #" << error << ": " << message << endl;
}

int main(int argc, const char* argv[]) {
    initialize(argc, argv);

    Scene scene;
	SceneLoader::loadScene("assets/scenes/big_scene.matte", scene);

    glEnable(GL_DEPTH_TEST);
    uint16_t frameCount = 0;
    double frameTimerSeconds = 0;
    double currentTime = 0, prevTime = glfwGetTime(), deltaTime;
    while (!glfwWindowShouldClose(GlobalAppState.window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        glfwPollEvents();

		scene.update(deltaTime);
		scene.render();

        glfwSwapBuffers(GlobalAppState.window);
        frameCount++;
        frameTimerSeconds += deltaTime;
        if (frameTimerSeconds > 1.0) {
            Logger::logInfo(std::to_string(frameCount) + " frames per second");
            frameCount = 0;
            frameTimerSeconds = 0;
		}
    }

    scene.free();
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

    GlobalAppState.window = glfwCreateWindow(GlobalAppState.width, GlobalAppState.height,
        GlobalAppState.title, nullptr, nullptr);
    if (!GlobalAppState.window) {
        cerr << "Error initializing GLFW window" << endl;
        return;
    }

	initializeInputSystem(GlobalAppState.window);
    glfwMakeContextCurrent(GlobalAppState.window);
	glfwSetWindowSizeCallback(GlobalAppState.window, setApplicationDimensions);
    glfwSwapInterval(0);

    // GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        cerr << "Unable to initialize GLEW: " << glewGetErrorString(err)
             << endl;
        exit(EXIT_FAILURE);
    }
}

void cleanup() {
    deallocateInputSystem();

    if (GlobalAppState.window) {
        glfwDestroyWindow(GlobalAppState.window);
    }

    glfwTerminate();
}
