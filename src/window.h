struct GLFWwindow;
class Window {
 public:
  Window(int width, int height, const char* title);
  ~Window();
  static void glfw_key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
  void setContext();
  void swapBuffers();
  bool isRunning();
 private:
  GLFWwindow * glfw_window = nullptr;

};
