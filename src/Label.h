#pragma once
#include "BoundText.h"
#include "Shader.h"

struct TextRenderer;
struct Label {
	BoundText bt;
	std::string text;
	glm::vec4 backgroundColor;
	glm::vec3 textColor;
};

void renderLabel(const Label& label, const Shader& shader, const TextRenderer& textRenderer);