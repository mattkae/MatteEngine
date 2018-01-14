#ifndef INPUT_H
#define INPUT_H

#include "gl_includes.h"
#include "Logger.h"

struct KeyState {
  bool hasChanged = false;
  bool isDown = false;
};

class Input {
 public:
  static Input* getInstance();
  bool is_up(int key);
  bool is_just_down(int key);
  bool is_down(int key);
  bool is_just_up(int key);
  
  static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
 private:
  static Input* mInstance;
  static const int NUM_KEYS = 512;
  KeyState mKeysPressed[NUM_KEYS];

  inline bool is_valid(int key);
};

#endif
