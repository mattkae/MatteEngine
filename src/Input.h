#ifndef INPUT_H
#define INPUT_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "BasicShapes.h"

struct KeyState {
  bool hasChanged = false;
  bool isDown = false;
};

class Input {
 public:
	static const Input& getInstance();
	bool isKeyUp(int key) const;
	bool isKeyJustDown(int key) const;
	bool isKeyDown(int key) const;
	bool isKeyJustUp(int key) const;
	inline bool isLeftClickDown() const { return mClickStates[0]; }
	inline bool isRightClickDown() const { return mClickStates[1]; }
	DoublePoint getCursorPosition() const;

	static void Initialize(GLFWwindow* window);
 private:
	static GLFWwindow* mWindow;
	static Input* mInstance;
	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static const int NUM_KEYS = 512;
	KeyState keyStates[NUM_KEYS];
	bool mClickStates[3];

	inline bool is_valid(int key) const;
};

#endif
