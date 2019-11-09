#pragma once
#include "../Shader.h"
#include "../TextRenderer.h"
#include "UIEvent.h"
#include "BoundText.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <functional>

enum TextInputType {
	TEXT = 0,
	INT = 1,
	FLOAT = 2
};

union TextInputValue {
	int* iVal;
	float* fVal;
	std::string* sVal;
};

struct TextInput {
	glm::vec3 textColor = glm::vec3(0, 0, 0);
	glm::vec4 backgroundColor = glm::vec4(1, 1, 1, 1);
	glm::vec4 focusedBackgroundColor = glm::vec4(1, 1, 1, 1);
	BoundText bt;
	int focusToken = -1;
	size_t cursorPosition = 0;
	bool isFocused = false;

	TextInputType inputType = TextInputType::TEXT;
	TextInputValue value;
	std::string representation = "";
};

void updateTextInput(TextInput& input, const TextRenderer& textRenderer);
void renderTextInput(const TextInput& input, const Shader& shader, const TextRenderer& textRenderer);