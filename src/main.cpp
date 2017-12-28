#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"

using namespace std;

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

// Window variables
int program_width = DEFAULT_WIDTH;
int program_height = DEFAULT_HEIGHT;
const char* WINDOW_TITLE = "Fluid Simulator";

void glfw_error_callback(int error, const char* message) {
  // TO-DO: Log the error
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  
}

void initialize_glfw(int argc, char** argv) {
  if (!glfwInit()) {
    // TO-DO: Add logging
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

  GLFWwindow* window = glfwCreateWindow(program_width, program_height, WINDOW_TITLE, NULL, NULL);

  if (!window) {
    // TO-DO: Add logging
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, glfw_key_callback);
}

int main(int argc, char** argv) {
  initialize_glfw(argc, argv);

  return 0;
}
