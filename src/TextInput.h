#pragma once
#include "Shader.h"
#include "BoundText.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

struct TextRenderer;
struct TextInput {
	glm::vec3 textColor = glm::vec3(0, 0, 0);
	glm::vec3 backgroundColor = glm::vec3(1, 1, 1);
	glm::vec3 focusedBackgroundColor = glm::vec3(1, 1, 1);
	std::string str = "";
	BoundText bt;
	int focusToken = -1;
	size_t cursorPosition = 0;
	glm::vec3 currentBackgroundColor = glm::vec3(1.f);
};

bool initializeTextInput(TextInput& input);
void updateTextInput(TextInput& input);
void renderTextInput(const TextInput& input, const Shader& shader);