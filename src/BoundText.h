#pragma once
#include "Rectangle.h"
#include "Shader.h"
#include "TextRenderer.h"
#include <glm/glm.hpp>
#include <string>

struct BoundText {
	GLfloat scale = 1.f;
	GLfloat padding = 0.f;
	Rectangle rect;
};

void renderBoundText(const BoundText& bt, const Shader& shader, const TextRenderer& textRenderer, std::string text,glm::vec4 backgroundColor, glm::vec3 textColor, GLint position = 0);
inline GLfloat getBoundTextHeight(const BoundText& bt, const TextRenderer& textRenderer) {
	return (textRenderer.getFontSize() + 2 * bt.padding) * bt.scale;
}