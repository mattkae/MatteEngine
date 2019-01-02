#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constants.h"
#include "Input.h"
#include "Scene.h"
#include <iostream>
#include <string>

using namespace std;

// Window variables
const char *WINDOWm_TITLE = "Matte 0.0.1";
GLFWwindow *window = nullptr;

void initialize(int argc, const char *argv[]);
void cleanup();

void glfw_error_callback(int error, const char *message) {
    cerr << "GLFW error #" << error << ": " << message << endl;
}

int main(int argc, const char *argv[]) {
    initialize(argc, argv);

    double currentTime = 0, prevTime = 0, deltaTime;
    Scene scene("assets/scenes/scene.json");

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        glfwPollEvents();
        if (Input::getInstance()->is_down(GLFW_KEY_R)) {
            scene.load_from_json("assets/scenes/scene.json");
        }

        scene.update(deltaTime);
        scene.render();

        glfwSwapBuffers(window);
    }

    cleanup();
    return 0;
}

void initialize(int argc, const char *argv[]) {
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

    window = glfwCreateWindow(Constants.width, Constants.height,
                              Constants.title.c_str(), nullptr, nullptr);
    if (!window) {
        cerr << "Error initializing GLFW window" << endl;
        return;
    }

    glfwSetKeyCallback(window, Input::glfw_key_callback);
    glfwMakeContextCurrent(window);

    // GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        cerr << "Unable to initialize GLEW: " << glewGetErrorString(err)
             << endl;
        exit(EXIT_FAILURE);
    }
}

void cleanup() {
    if (window) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}
