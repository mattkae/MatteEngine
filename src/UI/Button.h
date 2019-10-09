#pragma once
#include <functional>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../Shader.h"
#include "../TextRenderer.h"

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
	bool isClicked = false;
};

void updateButton(Button& button, const TextRenderer& textRenderer);
void renderButton(const Button& button, const Shader& shader, const TextRenderer& textRenderer);
inline GLfloat getButtonHeight(const Button& button, const TextRenderer& textRenderer) {
	return (textRenderer.getFontSize() + 2 * button.padding) * button.scale;
}