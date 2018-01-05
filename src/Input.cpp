#include "Input.h"
#include "Logger.h"

Input* Input::mInstance = nullptr;

Input* Input::getInstance() {
  if (!mInstance) {
    mInstance = new Input();
  }
  
  return mInstance;
}

bool Input::is_down(int key) {
  return is_valid(key) && mKeysPressed[key];
}

bool Input::is_up(int key) {
  return is_valid(key) && !mKeysPressed[key];
}

bool Input::is_valid(int key) {
  return key >= 0 && key < NUM_KEYS;
}

void Input::set_up(int key) {
  if (!is_valid(key)) return;
  
  mKeysPressed[key] = true;
}

void Input::set_down(int key) {
  if (!is_valid(key)) return;
  
  mKeysPressed[key] = false;
}

void Input::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  // Escape will always close the window
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }

  Input* instance = Input::getInstance();
  switch (action) {
  case GLFW_PRESS:
  case GLFW_REPEAT:
    instance->set_up(key);
    break;
  case GLFW_RELEASE:
  default:
    instance->set_down(key);
  }
}
