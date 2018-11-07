#define GLM_ENABLE_EXPERIMENTAL

#include "gl_includes.h"
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "Constants.h"
#include "Input.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "SceneReader.h"

using namespace std;

// Window variables
const char* WINDOWm_TITLE = "Fluid Simulator";
GLFWwindow* window = nullptr;

void initialize(int argc, const char* argv[]);
void cleanup();

void glfw_error_callback(int error, const char* message) {
	cerr << "GLFW error #" << error << ": " << message << endl;
}

int main(int argc, const char* argv[]) {
	initialize(argc, argv);

	double currentTime = 0, prevTime = 0, deltaTime;
	Scene scene = SceneReader::load_scene("assets/scenes/scene.json");


	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;
    
		glfwPollEvents();
		if (Input::getInstance()->is_down(GLFW_KEY_R)) {
			scene = SceneReader::load_scene("assets/scenes/scene.json");
		}

		scene.update(deltaTime);
		scene.render();

		glfwSwapBuffers(window);
	}

	cleanup();
	return 0;
}


void initialize(int argc, const char* argv[]) {
	// Constants
	boost::program_options::options_description desc{ "Options" };
	desc.add_options()
		("w,w", boost::program_options::value<int>()->default_value(800), "width")
		("h,h", boost::program_options::value<int>()->default_value(600), "height");

	boost::program_options::variables_map vm;
	boost::program_options::store(parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);

	Constants.width = vm["w"].as<int>();
	Constants.height = vm["h"].as<int>();
	Constants.aspectRatio = (float)Constants.width / (float)Constants.height;

	// GLFW
  if (!glfwInit()) {
    cerr << "Failed to initialize glfw." << endl;
    exit(EXIT_FAILURE);
  }
  
  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(Constants.width, Constants.height, Constants.title.c_str(), nullptr, nullptr);
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
