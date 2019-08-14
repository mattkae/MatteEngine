#include "Input.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Input* Input::mInstance = nullptr;
GLFWwindow* Input::mWindow = nullptr;

void Input::Initialize(GLFWwindow* window) {
	Input::mWindow = window;
	glfwSetKeyCallback(window, Input::glfwKeyCallback);
    glfwSetMouseButtonCallback(window, Input::glfwMouseButtonCallback);
}

const Input& Input::getInstance() {
  if (!mInstance) {
    mInstance = new Input();
  }
  
  return *mInstance;
}

bool Input::isKeyUp(int key) const {
  
  return is_valid(key) && (!keyStates[key].isDown);
}

bool Input::isKeyJustDown(int key) const {
  return is_valid(key) && (keyStates[key].isDown && keyStates[key].hasChanged);
}

bool Input::isKeyDown(int key) const {
  return is_valid(key) && (keyStates[key].isDown);
}

bool Input::isKeyJustUp(int key) const {
  return is_valid(key) && (!keyStates[key].isDown && keyStates[key].hasChanged);
}

bool Input::is_valid(int key) const {
  return key >= 0 && key < NUM_KEYS;
}

void Input::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  // Escape will always close the window
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }

  if (!Input::mInstance->is_valid(key)) return;
  
  switch (action) {
  case GLFW_REPEAT:
  case GLFW_PRESS:
    Input::mInstance->keyStates[key].hasChanged = !Input::mInstance->keyStates[key].isDown;
    Input::mInstance->keyStates[key].isDown = true;
    break;
  case GLFW_RELEASE:
  default:
    Input::mInstance->keyStates[key].hasChanged = Input::mInstance->keyStates[key].isDown;
    Input::mInstance->keyStates[key].isDown = false;
    break;
  }
}

void Input::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    const Input& instance = Input::getInstance();

	if (button > GLFW_MOUSE_BUTTON_3) {
		return;
	}
	
	switch (action) {
	case GLFW_PRESS:
	case GLFW_REPEAT:
		Input::mInstance->mClickStates[button] = true;
		break;
	case GLFW_RELEASE:
		Input::mInstance->mClickStates[button] = false;
		break;
	}
}

DoublePoint Input::getCursorPosition() const {
	DoublePoint point;
	if (Input::mWindow) {
		glfwGetCursorPos(Input::mWindow, &point.x, &point.y);
	}
	point.y = Constants.height - point.y;
	return point;
}
