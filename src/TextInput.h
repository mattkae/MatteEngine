#pragma once
#include "Shader.h"
#include "TextRenderer.h"
#include "UIEvent.h"
#include "BoundText.h"
#include <GL/glew.h>
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
	Vector4f textColor = Vector4f();
	Vector4f backgroundColor = Vector4f{ 1, 1, 1, 1 };
	Vector4f focusedBackgroundColor = Vector4f { 1, 1, 1, 1 };
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