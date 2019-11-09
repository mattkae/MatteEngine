#pragma once
#include "../Shader.h"
#include "Button.h"
#include "../TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "Label.h"
#include "../Shader.h"
#include "../LightweightArray.h"
#include <vector>
#include <variant>

struct UIElement {
	std::variant<TextInput, Button, Label> element;
	enum {
		BUTTON = 0,
		TEXT_INPUT = 1,
		LABEL = 2
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
	bool shouldOpen = false;
	LightweightArray<UIElement> uiElements;
	LightweightArray<UIContext*> deactivationDeps;
	GLfloat spaceBetweenElements = 2.f;
};

void updateUIContext(UIContext& context, const TextRenderer& textRenderer);
void renderUIContext(const UIContext& context, const Shader& shader, const TextRenderer& renderer);