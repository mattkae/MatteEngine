struct GLFWwindow;
class Window {
 public:
  Window(int width, int height, const char* title);
  ~Window();
  static void glfw_key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
  void set_context();
  void swap_buffers();
  bool is_running();
 private:
  GLFWwindow * glfw_window = nullptr;

};
