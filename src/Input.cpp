#include "Input.h"

bool Input::keys_pressed[Input::NUM_KEYS];

void Input::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
