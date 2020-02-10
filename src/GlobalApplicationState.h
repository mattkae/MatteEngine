#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <GL/glew.h>

struct GLFWwindow;

struct GlobalApplicationState {
  GLsizei width = 800;
  GLsizei height = 600;

  GLfloat floatWidth = 800.f;
  GLfloat floatHeight = 600.f;

  GLfloat aspectRatio = floatWidth / floatHeight;
  const char* title = "Matte";

  GLfloat near = 1.f;
  GLfloat far = 1000.f;
};

extern GlobalApplicationState GlobalAppState;

void setApplicationDimensions(GLFWwindow* window, int width, int height);

#endif
