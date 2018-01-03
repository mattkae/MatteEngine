#include "window.h"
#include "gl_includes.h"
#include "logger.h"
#include "Input.h"
#include <iostream>

/**
   Constructs a GLFW window with the given width, height,
   and title.
 */
Window::Window(int width, int height, const char* title) {
  this->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (this->glfw_window) {
    glfwSetKeyCallback(this->glfw_window, Input::glfw_key_callback);
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
void Window::set_context() {
  glfwMakeContextCurrent(this->glfw_window);
}

/**
   Swap the buffers on this window.
 */
void Window::swap_buffers() {
  glfwSwapBuffers(this->glfw_window);
}

bool Window::is_running() {
  return !glfwWindowShouldClose(this->glfw_window);
}
