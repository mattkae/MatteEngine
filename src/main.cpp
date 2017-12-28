#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "logger.h"
#include "window.h"
#include "input.h"

using namespace std;

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

// Window variables
int program_width = DEFAULT_WIDTH;
int program_height = DEFAULT_HEIGHT;
const char* WINDOW_TITLE = "Fluid Simulator";
Window* main_window = nullptr;

// Main loop variables

void glfw_error_callback(int error, const char* message) {
  // TO-DO: Log the error
  logger::log_error(message);
}

void initialize_glfw_environment(int argc, char** argv) {
  if (!glfwInit()) {
    // TO-DO: Add logging
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
  main_window = new Window(program_width, program_height, WINDOW_TITLE);
  main_window->setContext();
  
  while (main_window->isRunning()) {
    glClear(GL_COLOR_BUFFER_BIT);

    main_window->swapBuffers();

    glfwPollEvents();
  }

  // Free window memory
  delete main_window;

  // Terminate glfw
  glfwTerminate();
  return 0;
}
