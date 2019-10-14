#pragma once
#include "../Shader.h"
#include "Button.h"
#include "../TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "../Shader.h"
#include "../LightweightArray.h"
#include <vector>
#include <variant>

struct UIElement {
	std::variant<TextInput, Button> element;
	enum {
		BUTTON = 0,
		TEXT_INPUT = 1
	} type = UIElement::BUTTON;
};

// TODO: Implement directional contexts, and nested Panels
enum ContextDirection {
	VERTICAL = 0,
	HORIZONTAL = 1
};

struct UIContext {
	Panel panel;
	bool isActive = true;
	LightweightArray<UIElement> uiElements;
	GLfloat spaceBetweenElements = 2.f;
	LightweightArray<UIContext> dependentContexts;
};

void updateUIContext(UIContext& context, const TextRenderer& textRenderer);
void renderUIContext(const UIContext& context, const Shader& shader, const TextRenderer& renderer);