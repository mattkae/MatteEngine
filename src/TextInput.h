#pragma once
#include "Shader.h"
#include "TextRenderer.h"
#include "BoundText.h"
#include "MyString.h"
#include "UIEventType.h"
#include <GL/glew.h>

enum class TextInputType {
	NONE = 0,
	STRING = 1,
	INT = 2,
	FLOAT = 3
};

union TextInputValue {
	int iVal;
	float fVal;
};

struct UIEventProcessor;

struct TextInput {
	Vector4f textColor = Vector4f();
	Vector4f backgroundColor = Vector4f{ 1, 1, 1, 1 };
	Vector4f focusedBackgroundColor = Vector4f { 1, 1, 1, 1 };
	BoundText bt;
	int focusToken = -1;
	size_t cursorPosition = 0;
	bool isHovered = false;
	bool isFocused = false;

	TextInputType inputType = TextInputType::NONE;
	TextInputValue value;
	TextInputValue lastValue;
	StringBuilder representation;
	UIEventType eventType;

	void update(const TextRenderer& textRenderer, UIEventProcessor* processor);
	void render(const Shader& shader, const TextRenderer& textRenderer);
	void free();
};