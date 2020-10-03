#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "App.h"
#include "Logger.h"
#include <iostream>
#include <string>

Application GlobalApp;

using namespace std;

void initialize(int argc, const char* argv[]);
void cleanup();

void glfw_error_callback(int error, const char* message) {
    cerr << "GLFW error #" << error << ": " << message << endl;
}

int main(int argc, const char* argv[]) {
    initialize(argc, argv);

    uint16_t frameCount = 0;
    double frameTimerSeconds = 0;
    double currentTime = 0, prevTime = glfwGetTime(), deltaTime;
    while (!glfwWindowShouldClose(GlobalApp.window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        glfwPollEvents();

		GlobalApp.update(deltaTime);
		GlobalApp.render();

        glfwSwapBuffers(GlobalApp.window);
        frameCount++;
        frameTimerSeconds += deltaTime;
        if (frameTimerSeconds > 1.0) {
            Logger::logInfo(std::to_string(frameCount) + " frames per second");
            frameCount = 0;
            frameTimerSeconds = 0;
		}
    }

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
    GlobalApp.initializeWindow();
    if (!GlobalApp.window) {
        cerr << "Error initializing GLFW window" << endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(GlobalApp.window);
	glfwSetWindowSizeCallback(GlobalApp.window, setApplicationDimensions);
    glfwSwapInterval(0);

    // GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        cerr << "Unable to initialize GLEW: " << glewGetErrorString(err) << endl;
        exit(EXIT_FAILURE);
    }

    // App initialization
    GlobalApp.initialize();
}

void cleanup() {
    GlobalApp.free();
    glfwTerminate();
}
