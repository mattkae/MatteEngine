#include "gl_includes.h"

namespace input {
  // Keep track of key state globally
  const int NUM_KEYS = 512;
  static bool keys_pressed[NUM_KEYS];

  /**
     Sets the state of the key when input is received.
   */
  static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Escape will always close the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
    
    if (action == GLFW_PRESS) {
      keys_pressed[key] = true;
    } else {
      keys_pressed[key] = false;
    }
  }
}
