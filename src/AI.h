#ifndef INPUT_H
#define INPUT_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct KeyState {
  bool hasChanged = false;
  bool isDown = false;
};

struct MousePosition {
	double x = 0;
	double y = 0;
};

struct ClickState: MousePosition {
	glm::vec2 position;
    bool isDown = false;
};

class Input {
 public:
  static Input* getInstance();
  bool isKeyUp(int key);
  bool isKeyJustDown(int key);
  bool isKeyDown(int key);
  bool isKeyJustUp(int key);
  inline const ClickState& getLeftClickState() { return clickStates[0]; }
  inline const ClickState& getRightClickState() { return clickStates[1]; }
  inline MousePosition getCursorPosition();

  static GLFWwindow* gWindow;

  static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
 private:
  static Input* mInstance;
  static const int NUM_KEYS = 512;
  KeyState keyStates[NUM_KEYS];
  ClickState clickStates[3];

  inline bool is_valid(int key);
};

#endif
