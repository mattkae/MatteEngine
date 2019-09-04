#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "BasicShapes.h"
#include "Shader.h"

struct RenderableRectangle {
	Rectangle r;
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
void renderRenderableRectangle(const RenderableRectangle& rect, const Shader& shader, glm::vec3 backgroundColor);