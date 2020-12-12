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

struct TextureDebugView {
	bool show = false;
	Rectangle rectangle = { 8.f, 0.f, 240.f, 150.f };
	GLuint texture;
	void render() const;
};

struct Scrollbar {
	bool show = false;
	bool isClicked = false;
	Vector2f originalClickSpot;
	Rectangle rectangle;
	GLfloat yOffset = 0;
	int inputToken;
};

/**
 * Manages a selection of UI Elements inside of a single panel.
*/
struct UIContext {
	Panel panel;
	bool shouldOpen = false;
	bool shouldClose = false;
	bool isClosable = true;
	GLfloat spaceBetweenElements = 2.f;
	GLfloat scrollAmount = 0;

	Button closeButton;
	List<UIElement> uiElements;
	TextureDebugView textureDebugger;
	Scrollbar scrollbar;

	bool isOpen();
	void init();
	void update(float dtMs, const TextRenderer& textRenderer);
	void render(const Shader& shader, const TextRenderer& renderer) const;
	void free();
};
