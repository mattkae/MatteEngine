#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "Logger.h"
#include "Constants.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "LightSystem.h"
#include "Input.h"
#include "ImageDrawer.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// Window variables
const char* WINDOW_TITLE = "Fluid Simulator";
GLFWwindow* window = nullptr;

void initialize(int argc, char** argv);
void cleanup();

void glfw_error_callback(int error, const char* message) {
  cerr << "GLFW error #" << error << ": " << message << endl;
}

int main(int argc, char** argv) {
  initialize(argc, argv);

  double currentTime = 0, prevTime = 0, deltaTime;
  ImageDrawer imgDrawer;

  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    
    glfwPollEvents();

    scene.update(deltaTime);
    scene.render();
    
    glfwSwapBuffers(window);
  }

  cleanup();
  return 0;
}


void initialize(int argc, char** argv) {
  if (!glfwInit()) {
    logger::log_error("Failed to initialize glfw.");
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(Constants.width, Constants.height, Constants.title, nullptr, nullptr);
  if (!window) {
    cerr << "Error initializing GLFW window" << endl;
    return;
  }

  glfwSetKeyCallback(window, Input::glfw_key_callback);
  glfwMakeContextCurrent(window);

  // GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    logger::log_error((const char*)glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }
}

void cleanup() {
  if (window) {
    glfwDestroyWindow(window);
  }
  
  glfwTerminate();
}
