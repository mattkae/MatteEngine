#include "window.h"
#include "gl_includes.h"
#include "logger.h"
#include "input.h"
#include <iostream>

/**
   Constructs a GLFW window with the given width, height,
   and title.
 */
Window::Window(int width, int height, const char* title) {
  this->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (this->glfw_window) {
    glfwSetKeyCallback(this->glfw_window, input::glfw_key_callback);
  } else {
    // TO-DO: Log error    
    logger::log_error("Error while initializing GLFW window");
  }
}

/**
   Default destructor for the window.
 */
Window::~Window() {
  if (this->glfw_window) {
    glfwDestroyWindow(this->glfw_window);
  }
}

/**
   Set the window pointed to by this object as the current context.
 */
void Window::setContext() {
  glfwMakeContextCurrent(this->glfw_window);
}

/**
   Swap the buffers on this window.
 */
void Window::swapBuffers() {
  glfwSwapBuffers(this->glfw_window);
}

bool Window::isRunning() {
  return !glfwWindowShouldClose(this->glfw_window);
}
