#pragma once
#include "../Shader.h"
#include "../TextRenderer.h"
#include "BoundText.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <functional>

struct TextInput {
	glm::vec3 textColor = glm::vec3(0, 0, 0);
	glm::vec4 backgroundColor = glm::vec4(1, 1, 1, 1);
	glm::vec4 focusedBackgroundColor = glm::vec4(1, 1, 1, 1);
	std::string str = "";
	BoundText bt;
	int focusToken = -1;
	size_t cursorPosition = 0;
	std::function<void(std::string str)> onChangeHandler;
	bool isFocused = false;
};

void updateTextInput(TextInput& input, const TextRenderer& textRenderer);
void renderTextInput(const TextInput& input, const Shader& shader, const TextRenderer& textRenderer);