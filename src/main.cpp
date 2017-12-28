#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "logger.h"
#include "window.h"

using namespace std;

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

// Window variables
int program_width = DEFAULT_WIDTH;
int program_height = DEFAULT_HEIGHT;
const char* WINDOW_TITLE = "Fluid Simulator";
Window* main_window = nullptr;

// Main loop variables
bool is_running = false;

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  // TO-DO: Implement key input
}

void glfw_error_callback(int error, const char* message) {
  // TO-DO: Log the error
  logger::LogError(message);
}

void initialize_glfw(int argc, char** argv) {
  if (!glfwInit()) {
    // TO-DO: Add logging
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
}

int main(int argc, char** argv) {
  initialize_glfw(argc, argv);
  main_window = new Window(program_width, program_height, WINDOW_TITLE);
  main_window->setContext();
  is_running = main_window->isCreated();
  
  while (is_running) {
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
