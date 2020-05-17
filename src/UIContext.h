#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "Label.h"
#include "Shader.h"
#include "List.h"
#include <vector>

enum UIElementType {
	BUTTON = 0,
	TEXT_INPUT = 1,
	LABEL = 2
};


struct UIElementUnion {
	UIElementType elementType;

	UIElementUnion() { }

	UIElementUnion(UIElementType type) {
		elementType = type;
		switch (type) {
		case UIElementType::BUTTON:
			button = Button();
			break;
		case UIElementType::TEXT_INPUT:
			textInput = TextInput();
			break;
		case UIElementType::LABEL:
			label = Label();
			break;
		}
	}

	~UIElementUnion() {
		switch (elementType) {
		case UIElementType::BUTTON:
			break;
		case UIElementType::TEXT_INPUT:
			break;
		case UIElementType::LABEL:
			break;
		}
	}

	TextInput textInput;
	Button button;
	Label label;
};

struct UIElement {
	UIElementUnion element;
	UIElementType elementType;

	UIElement() { }

	UIElement(UIElementType type) {
		elementType = type;
		element = UIElementUnion(type);
	}

	~UIElement() { }
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
	bool shouldClose = false;
	GLfloat spaceBetweenElements = 2.f;
	List<UIElement> uiElements;

	void update(const TextRenderer& textRenderer);
	void render(const Shader& shader, const TextRenderer& renderer) const;
	void free();
};
