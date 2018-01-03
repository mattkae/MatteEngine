#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "Logger.h"
#include "Window.h"
#include "Input.h"
#include "Application.h"

using namespace std;

// Window variables
const char* WINDOW_TITLE = "Fluid Simulator";
Window* main_window = nullptr;

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
  initialize_glfw_environment(argc, argv);
  main_window = new Window(Application::screenWidth, Application::screenHeight, WINDOW_TITLE);
  main_window->set_context();
  
  while (main_window->is_running()) {
    glClear(GL_COLOR_BUFFER_BIT);

    main_window->swap_buffers();

    glfwPollEvents();
  }

  // Free window memory
  delete main_window;

  // Terminate glfw
  glfwTerminate();
  return 0;
}
