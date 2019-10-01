#pragma once
#include <functional>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "InteractableRectangle.h"
#include "Shader.h"

struct TextRenderer;

struct Button {
	std::function<void(void)> onClickHandler;
	std::string label = "";
	GLfloat padding = 0;
	glm::vec2 position = glm::vec2(0);
	GLfloat scale = 1.0;
	glm::vec4 buttonColor = glm::vec4(0);
	glm::vec4 hoverColor = glm::vec4(0);
	glm::vec4 textColor = glm::vec4(0);
	glm::vec4 borderColor = glm::vec4(0);
	GLfloat width = 128.f;

	InteractableRectangle rect;
	glm::vec2 mTextPosition = glm::vec2(0);
};

void initializeButton(Button& button, const TextRenderer& textRenderer);
void updateButton(Button& button);
void renderButton(const Button& button, const Shader& shader, const TextRenderer& textRenderer);