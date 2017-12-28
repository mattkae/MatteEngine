#ifdef _WIN32
#ifdef _WIN64

#else

#endif
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#elif __linux__

#else

#endif

#include <iostream>
using namespace std;

void glfw_error_callback(int error, const char* message) {
  // Log the error
}

void initialize_glfw(int argc, char** argv) {
  if (!glfwInit()) {
    // TO-DO: Add logign
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(glfw_error_callback);
}

int main(int argc, char** argv) {
  initialize_glfw(argc, argv);

  return 0;
}
