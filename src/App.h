#ifndef APP_H
#define APP_H
#include "Editor.h"
#include "Scene.h"
#include <GL/glew.h>

struct GLFWwindow;

struct Application {
	GLsizei width = 1280;
	GLsizei height = 960;

	GLfloat floatWidth = 1280.f;
	GLfloat floatHeight = 960.f;

	GLfloat aspectRatio = floatWidth / floatHeight;
	const char* title = "Matte";

	GLfloat near = 1.f;
	GLfloat far = 1000.f;
	GLFWwindow* window = nullptr;

	Editor editor;
	Scene scene;

	void initializeWindow();
	void initialize();
	void update(double dt);
	void render();
	void free();
	int castRayToModel();
};

void setApplicationDimensions(GLFWwindow* window, int width, int height);

extern Application GlobalApp;

#endif
