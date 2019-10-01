#pragma once
#include "../Shader.h"
#include "BoundText.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

struct TextRenderer;
struct TextInput {
	glm::vec3 textColor = glm::vec3(0, 0, 0);
	glm::vec4 backgroundColor = glm::vec4(1, 1, 1, 1);
	glm::vec4 focusedBackgroundColor = glm::vec4(1, 1, 1, 1);
	std::string str = "";
	BoundText bt;
	int focusToken = -1;
	size_t cursorPosition = 0;
	glm::vec4 currentBackgroundColor = glm::vec4(1.f);
};

bool initializeTextInput(TextInput& input);
void updateTextInput(TextInput& input);
void renderTextInput(const TextInput& input, const Shader& shader, const TextRenderer& textRenderer);