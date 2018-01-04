#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "Logger.h"
#include "Window.h"
#include "Application.h"

using namespace std;

// Window variables
const char* WINDOW_TITLE = "Fluid Simulator";
Window* mainWindow = nullptr;

void glfw_error_callback(int error, const char* message) {
  logger::log_error(message);
}

void initialize_glfw_environment(int argc, char** argv) {
  if (!glfwInit()) {
    logger::log_error("Failed to initialize glfw.");
    exit(EXIT_FAILURE);
  }
  
  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main(int argc, char** argv) {
  // Initialization
  initialize_glfw_environment(argc, argv);
  mainWindow = new Window(Application::screenWidth, Application::screenHeight, WINDOW_TITLE);
  mainWindow->set_context();

  // Loop variables
  double currentTime = 0, prevTime = 0, deltaTime;
  
  while (mainWindow->is_running()) {
    // Update timestep
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    
    // Get Input
    glfwPollEvents();

    // Update

    // Render
    glClear(GL_COLOR_BUFFER_BIT);
    mainWindow->swap_buffers();
  }

  // Free window memory
  delete mainWindow;

  // Terminate glfw
  glfwTerminate();
  return 0;
}
