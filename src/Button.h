#pragma once
#include <functional>
#include <string>
#include <GL/glew.h>
#include "Shader.h"
#include "TextRenderer.h"
#include "Vector4f.h"
#include "Vector2f.h"
#include "Rectangle.h"

struct TextRenderer;

struct Button {
	std::string label = "";
	GLfloat padding = 0;
	Vector2f position;
	GLfloat scale = 1.0;
	Vector4f buttonColor;
	Vector4f hoverColor;
	Vector4f textColor;
	Vector4f borderColor;
	GLfloat width = 128.f;
	bool* isClicked = nullptr;
	Rectangle boundingRect;
	
	void update(const TextRenderer& textRenderer);
	void render(const Shader& shader, const TextRenderer& textRenderer);
};

inline GLfloat getButtonHeight(const Button& button, const TextRenderer& textRenderer) {
	return (textRenderer.getFontSize() + 2 * button.padding) * button.scale;
}