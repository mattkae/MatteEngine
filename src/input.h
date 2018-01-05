#ifndef INPUT_H
#define INPUT_H

#include "gl_includes.h"
#include "Logger.h"

class Input {
 public:
  // Keep track of key state globally
  static const int NUM_KEYS = 512;
  static bool keys_pressed[];
  static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
