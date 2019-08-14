#include "Input.h"
#include <iostream>

using namespace std;

Input* Input::mInstance = nullptr;

Input* Input::getInstance() {
  if (!mInstance) {
    mInstance = new Input();
  }
  
  return mInstance;
}

bool Input::isKeyUp(int key) {
  
  return is_valid(key) && (!keyStates[key].isDown);
}

bool Input::isKeyJustDown(int key) {
  // Consume the key change
  bool result = is_valid(key) && (keyStates[key].isDown && keyStates[key].hasChanged);
  keyStates[key].hasChanged = false;
  return result;
}

bool Input::isKeyDown(int key) {
  return is_valid(key) && (keyStates[key].isDown);
}

bool Input::isKeyJustUp(int key) {
  // Consume the key change
  bool result = is_valid(key) && (!keyStates[key].isDown && keyStates[key].hasChanged);
  keyStates[key].hasChanged = false;
  return result;
}

bool Input::is_valid(int key) {
  return key >= 0 && key < NUM_KEYS;
}

void Input::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
    instance->keyStates[key].hasChanged = !instance->keyStates[key].isDown;
    instance->keyStates[key].isDown = true;
    break;
  case GLFW_RELEASE:
  default:
    instance->keyStates[key].hasChanged = instance->keyStates[key].isDown;
    instance->keyStates[key].isDown = false;
    break;
  }
}

void Input::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Input* instance = Input::getInstance();

	if (button > GLFW_MOUSE_BUTTON_3) {
		return;
	}
	
	switch (action) {
	case GLFW_PRESS:
	case GLFW_REPEAT:
		glfwGetCursorPos(window, &instance->clickStates[button].x, &instance->clickStates[button].y);
		instance->clickStates[button].isDown = true;
		break;
	case GLFW_RELEASE:
        glfwGetCursorPos(window, &instance->clickStates[button].x, &instance->clickStates[button].y);
        instance->clickStates[button].isDown = false;
		break;
	}
}

MousePosition Input::getCursorPosition() {
	MousePosition coords;
	glfwGetCursorPos(Input::gWindow, &coords.x, &coords.y);
	return coords;
}

GLFWwindow* Input::gWindow = nullptr;