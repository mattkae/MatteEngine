#pragma once
#include "../Primitives/Rectangle.h"
#include "../Shader.h"
#include <glm/glm.hpp>
#include <string>

struct TextRenderer;
struct BoundText {
	GLfloat scale = 1.f;
	GLfloat padding = 0.f;
	Rectangle rect;
};

void renderBoundText(const BoundText& bt, const Shader& shader, const TextRenderer& textRenderer, std::string text,glm::vec4 backgroundColor, glm::vec3 textColor, GLint position = 0);