#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "Constants.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "LightSystem.h"
#include "Input.h"
#include "ImageDrawer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

// Window variables
const char* WINDOW_TITLE = "Fluid Simulator";
GLFWwindow* window = nullptr;

void initialize(int argc, char** argv);
void cleanup();

void glfw_error_callback(int error, const char* message) {
  cerr << "GLFW error #" << error << ": " << message << endl;
}

int main(int argc, char** argv) {
  initialize(argc, argv);

  Camera camera(glm::vec3(0, 3, 10));
  Shader shader("assets/shader.vert", "assets/shader.frag");
  Shader shadowShader("assets/shadows.vert", "assets/shadows.frag");
  Model model("assets/test.obj");
  model.set_model(glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)));
  Model floor("assets/floor.obj");
  floor.set_model(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -3.0, 0.0)));  

  LightSystem lightSystem;
  lightSystem.set_ambient(glm::vec3(0.1, 0.1, 0.1));
  //lightSystem.add_directional(glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
  //lightSystem.add_point(glm::vec3(0.0, 5.0, 2.0), glm::vec3(1.0, 1.0, 1.0), 0.55f, 0.2f, 0.11f);
  lightSystem.add_spot(glm::vec3(0.0, -1.0, 0.0),
		       glm::vec3(0.0, 5.0, 0.0),
		       glm::vec3(1.0, 1.0, 1.0),
		       0.55f, 0.001f, 0.0001f, 25.71, 2.f
		       );
  
  // Loop variables
  double currentTime = 0, prevTime = 0, deltaTime;
  ImageDrawer imgDrawer;

  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    // Update timestep
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    
    // Get Input
    glfwPollEvents();

    // Turn lights on and off
    Input* i = Input::getInstance();
    if (i->is_just_down(GLFW_KEY_1)) {
      lightSystem.toggle(0);
    }
    if (i->is_just_down(GLFW_KEY_2)) {
      lightSystem.toggle(1);
    }
    if (i->is_just_down(GLFW_KEY_3)) {
      lightSystem.toggle(2);
    }

    // Update
    move_camera(deltaTime, &camera);
    camera.update(deltaTime);

    // Render shadows
    lightSystem.render_shadows(&shadowShader, &model, &floor);

    // Render
    shader.Use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.render(&shader);
    lightSystem.render(&shader);
    model.render(&shader);
    floor.render(&shader);
    glfwSwapBuffers(window);
  }

  cleanup();
  return 0;
}


void initialize(int argc, char** argv) {
  if (!glfwInit()) {
    cerr << "Failed to initialize glfw." << endl;
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(Constants.width, Constants.height, Constants.title, nullptr, nullptr);
  if (!window) {
    cerr << "Error initializing GLFW window" << endl;
    return;
  }

  glfwSetKeyCallback(window, Input::glfw_key_callback);
  glfwMakeContextCurrent(window);

  // GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    cerr << "Unable to initialize GLEW: " << glewGetErrorString(err) << endl;
    exit(EXIT_FAILURE);
  }
}

void cleanup() {
  if (window) {
    glfwDestroyWindow(window);
  }
  
  glfwTerminate();
}
