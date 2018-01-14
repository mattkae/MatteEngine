#include "Input.h"
#include "Logger.h"

Input* Input::mInstance = nullptr;

Input* Input::getInstance() {
  if (!mInstance) {
    mInstance = new Input();
  }
  
  return mInstance;
}

bool Input::is_up(int key) {
  
  return is_valid(key) && (!mKeysPressed[key].isDown);
}

bool Input::is_just_down(int key) {
  // Consume the key change
  bool result = is_valid(key) && (mKeysPressed[key].isDown && mKeysPressed[key].hasChanged);
  mKeysPressed[key].hasChanged = false;
  return result;
}

bool Input::is_down(int key) {
  return is_valid(key) && (mKeysPressed[key].isDown);
}

bool Input::is_just_up(int key) {
  // Consume the key change
  bool result = is_valid(key) && (!mKeysPressed[key].isDown && mKeysPressed[key].hasChanged);
  mKeysPressed[key].hasChanged = false;
  return result;
}

bool Input::is_valid(int key) {
  return key >= 0 && key < NUM_KEYS;
}

void Input::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  // Escape will always close the window
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }

  Input* instance = Input::getInstance();
  if (!instance->is_valid(key)) return;
  
  switch (action) {
  case GLFW_REPEAT:
  case GLFW_PRESS:
    instance->mKeysPressed[key].hasChanged = !instance->mKeysPressed[key].isDown;
    instance->mKeysPressed[key].isDown = true;
    break;
  case GLFW_RELEASE:
  default:
    instance->mKeysPressed[key].hasChanged = instance->mKeysPressed[key].isDown;
    instance->mKeysPressed[key].isDown = false;
    break;
  }
}
