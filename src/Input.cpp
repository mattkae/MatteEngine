#include "Input.h"
#include "App.h"
#include "Logger.h"
#include <iostream>
#include <GLFW/glfw3.h>

const int NUM_KEYS = 512;
const int NUM_BUTTONS = 3;

struct KeyState {
	bool hasChanged = false;
	bool isDown = false;
};

struct InputState {
	GLFWwindow* window = nullptr;
	KeyState keyStates[NUM_KEYS];
	bool clickStates[NUM_BUTTONS];
	int focusToken = 0;
	int nextFocusToken = 1;
	int currentKeyDown = -1;
	int currentScancode = -1;

	// Cursors
	GLFWcursor* defaultCursor = nullptr;
	GLFWcursor* textCursor = nullptr;
};

InputState globalInput;

inline bool isKeyValid(int key) {
  return key >= 0 && key < NUM_KEYS;
}

inline bool checkFocusToken(int focusToken) {
	return focusToken == globalInput.focusToken;
}

void resetFocus() {
	globalInput.focusToken = DEFAULT_FOCUS_TOKEN;
}

int grabFocus() {
	globalInput.focusToken = globalInput.nextFocusToken++;
	return globalInput.focusToken;
}

void returnFocus(int focusToken) {
	if (globalInput.focusToken == focusToken) {
		globalInput.focusToken = DEFAULT_FOCUS_TOKEN;
	}
}

int getInputFocus() {
	return globalInput.focusToken;
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Escape will always close the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	if (!isKeyValid(key)) return;
  
	switch (action) {
		case GLFW_REPEAT:
		case GLFW_PRESS:
			globalInput.currentKeyDown = key;
			globalInput.currentScancode = scancode;
			globalInput.keyStates[key].hasChanged = !globalInput.keyStates[key].isDown;
			globalInput.keyStates[key].isDown = true;
			break;
		case GLFW_RELEASE:
		default:
			globalInput.currentKeyDown = -1;
			globalInput.currentScancode = -1;
			globalInput.keyStates[key].hasChanged = globalInput.keyStates[key].isDown;
			globalInput.keyStates[key].isDown = false;
			break;
	}
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button > GLFW_MOUSE_BUTTON_3) {
		return;
	}
	
	switch (action) {
	case GLFW_PRESS:
	case GLFW_REPEAT:
		globalInput.clickStates[button] = true;
		break;
	case GLFW_RELEASE:
		globalInput.clickStates[button] = false;
		break;
	}
}

void initializeInputSystem(GLFWwindow* window) {
	globalInput.window = window;
	glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);

	globalInput.defaultCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	globalInput.textCursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
}

void deallocateInputSystem() {
	if (globalInput.defaultCursor) {
		glfwDestroyCursor(globalInput.defaultCursor);
	}
	if (globalInput.textCursor) {
		glfwDestroyCursor(globalInput.textCursor);
	}
}

bool isKeyUp(int key, int focusToken) {
	return checkFocusToken(focusToken) && isKeyValid(key) && (!globalInput.keyStates[key].isDown);
}

bool isKeyJustDown(int key, int focusToken) {
	// Consume the "hasChanged" modifier on that key
	if (checkFocusToken(focusToken) && isKeyValid(key) && (globalInput.keyStates[key].isDown && globalInput.keyStates[key].hasChanged)) {
		 globalInput.keyStates[key].hasChanged = false;
		 return true;
	}

	return false;
}

bool isKeyDown(int key, int focusToken) {
	return checkFocusToken(focusToken) && isKeyValid(key) && (globalInput.keyStates[key].isDown);
}

bool isKeyJustUp(int key, int focusToken) {
	return checkFocusToken(focusToken) && isKeyValid(key) && (!globalInput.keyStates[key].isDown && globalInput.keyStates[key].hasChanged);
}

bool isLeftClickDown() {
	return globalInput.clickStates[GLFW_MOUSE_BUTTON_LEFT];
}

bool isRightClickDown() {
	return globalInput.clickStates[GLFW_MOUSE_BUTTON_RIGHT];
}

Vector2f getCursorPosition() {
	Vector2f point;
	if (globalInput.window) {
		double x, y;
		glfwGetCursorPos(globalInput.window, &x, &y);
		point.x = static_cast<float>(x);
		point.y = static_cast<float>(y);
	}
	point.y = GlobalApp.floatHeight - point.y;
	return point;
}

int getCurrentKeyDown(int focusToken) {
	return focusToken == globalInput.focusToken ? globalInput.currentKeyDown : -1;
}

int getCurrentScancode(int focusToken) {
	return focusToken == globalInput.focusToken ? globalInput.currentScancode : -1;
}

void getWindowDimensions(int& width, int& height) {
	glfwGetWindowSize(globalInput.window, &width, &height);
}

bool isDefaultFocused() {
	return checkFocusToken(DEFAULT_FOCUS_TOKEN);
}

void setCursorToText() {
	glfwSetCursor(globalInput.window, globalInput.textCursor);
}

void resetCursor() {
	glfwSetCursor(globalInput.window, globalInput.defaultCursor);
}