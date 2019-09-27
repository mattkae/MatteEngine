#include "GlobalApplicationState.h"
#include "Input.h"

GlobalApplicationState GlobalAppState;

void setApplicationDimensions(GLFWwindow* window, int width, int height) {
	if (width == GlobalAppState.width && height == GlobalAppState.height) {
		return;
	}

	GlobalAppState.width = width;
	GlobalAppState.height = height;
	GlobalAppState.floatWidth = static_cast<GLfloat>(width);
	GlobalAppState.floatHeight = static_cast<GLfloat>(height);
	GlobalAppState.aspectRatio = GlobalAppState.floatWidth / GlobalAppState.floatHeight;
	glViewport(0, 0, GlobalAppState.floatWidth, GlobalAppState.floatHeight);
}