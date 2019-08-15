#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "BasicShapes.h"

class Shader;
struct RenderableRectangle {
	Rectangle r;
	glm::vec3 backgroundColor = glm::vec3(1);
	glm::vec3 borderColor = glm::vec3(1);
	bool isFocused = false;
	bool isClicked = false;
	bool isJustClicked = false;
	bool isHovered = false;

	GLuint mVao = 0;
	GLuint mVbo = 0;
	GLuint mEbo = 0;
};

void initializeRenderableRectangle(RenderableRectangle& rect);
void updateRenderableRectangle(RenderableRectangle& rect);
void renderRenderableRectangle(const RenderableRectangle& rect, const Shader& shader);