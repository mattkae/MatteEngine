#ifndef INPUT_H
#define INPUT_H

#include "gl_includes.h"
#include "Logger.h"

class Input {
 public:
  static Input* getInstance();
  bool is_down(int key);
  bool is_up(int key);
  void set_up(int key);
  void set_down(int key);
  
  static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
 private:
  static Input* mInstance;
  static const int NUM_KEYS = 512;
  bool mKeysPressed[NUM_KEYS];

  inline bool is_valid(int key);
};

#endif
