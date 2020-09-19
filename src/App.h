#ifndef APP_H
#define APP_H
#include "EditorUI.h"
#include "Scene.h"
#include <GL/glew.h>

struct GLFWwindow;

struct Application {
	GLsizei width = 800;
	GLsizei height = 600;

	GLfloat floatWidth = 800.f;
	GLfloat floatHeight = 600.f;

	GLfloat aspectRatio = floatWidth / floatHeight;
	const char* title = "Matte";

	GLfloat near = 1.f;
	GLfloat far = 1000.f;
	GLFWwindow* window = nullptr;

	EditorUI editorUI;
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
