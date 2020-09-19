#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "Label.h"
#include "Shader.h"
#include "List.h"
#include "UIElement.h"

/**
 * Manages a selection of UI Elements inside of a single panel.
*/
struct UIContext {
	Panel panel;
	bool shouldOpen = false;
	bool shouldClose = false;
	bool isClosable = true;
	GLfloat spaceBetweenElements = 2.f;

	GLfloat startX = 0;
	GLfloat currentX = 0;
	GLfloat endX = 0;

	Button closeButton;
	List<UIElement> uiElements;

	bool isOpen();
	void init();
	void update(float dtMs, const TextRenderer& textRenderer, UIEventProcessor* processor);
	void render(const Shader& shader, const TextRenderer& renderer) const;
	void free();
};
