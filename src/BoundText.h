#pragma once
#include "InteractableRectangle.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <string>

struct TextRenderer;
struct BoundText {
	GLfloat scale = 1.f;
	glm::vec2 padding = glm::vec2(0.f);
	InteractableRectangle box;
};

void updateBoundText(BoundText& bt);
void renderBoundText(const BoundText& bt, const Shader& shader, const TextRenderer& textRenderer, std::string text,glm::vec4 backgroundColor, glm::vec3 textColor, GLint position = 0);