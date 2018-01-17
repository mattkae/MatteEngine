#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "Logger.h"
#include "Window.h"
#include "Application.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "LightSystem.h"
#include "Input.h"
#include "Texture.h"
#include "ImageDrawer.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// Window variables
const char* WINDOW_TITLE = "Fluid Simulator";
Window* mainWindow = nullptr;

void glfw_error_callback(int error, const char* message) {
  logger::log_error(message);
}

void initialize_glfw_environment(int argc, char** argv) {
  if (!glfwInit()) {
    logger::log_error("Failed to initialize glfw.");
    exit(EXIT_FAILURE);
  }
  
  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initialize_glew() {
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    logger::log_error((const char*)glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char** argv) {
  // Initialization
  initialize_glfw_environment(argc, argv);

  mainWindow = new Window(Application::screenWidth, Application::screenHeight, WINDOW_TITLE);
  mainWindow->set_context();

  initialize_glew();

  /*
  Camera camera;
  Shader shader("assets/shader.vert", "assets/shader.frag");
  Shader shadowShader("assets/shadows.vert", "assets/shadows.frag");
  Model model("assets/cube.obj");
  Model floor("assets/floor.obj");
floor.set_model(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -3.0, 0.0)));
  */
  Texture texture("assets/flower.jpg");
  ImageDrawer imgDrawer(&texture);
  
  

  //  LightSystem lightSystem;
  //lightSystem.set_ambient(glm::vec3(0.1, 0.1, 0.1));
  //lightSystem.add_directional(glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
  //lightSystem.add_point(glm::vec3(0.0, 5.0, 2.0), glm::vec3(1.0, 1.0, 1.0), 0.55f, 0.2f, 0.11f);
  //lightSystem.add_spot(glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 5.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.99f, 0.2f, 0.23f, 0.85f, 2.f);
  
  // Loop variables
  double currentTime = 0, prevTime = 0, deltaTime;
  //glEnable(GL_DEPTH_TEST);
  
  while (mainWindow->is_running()) {
    // Update timestep
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    
    // Get Input
    glfwPollEvents();

    /* Turn lights on and off
    Input* i = Input::getInstance();
    if (i->is_just_down(GLFW_KEY_1)) {
      lightSystem.toggle(0);
    }
    if (i->is_just_down(GLFW_KEY_2)) {
      lightSystem.toggle(1);
    }
    if (i->is_just_down(GLFW_KEY_3)) {
      lightSystem.toggle(2);
      }*/

    // Update
    //move_camera(deltaTime, &camera);
    //camera.update(deltaTime);

    // Render shadows
    // shadowShader.Use();
    //lightSystem.render_shadows(&shadowShader, &model, &floor);

    imgDrawer.render();
    mainWindow->swap_buffers();
    
    // Render
    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.Use();
    camera.render(&shader);
    lightSystem.render(&shader);
    model.render(&shader);
    floor.render(&shader);
    mainWindow->swap_buffers();
    glUseProgram(0);*/
  }

  // Free window memory
  delete mainWindow;

  // Terminate glfw
  glfwTerminate();
  return 0;
}
