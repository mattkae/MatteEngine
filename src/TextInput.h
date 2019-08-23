#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "RenderableRectangle.h"

struct TextRenderer;
class Shader;
struct TextInput {
	TextRenderer* textRenderer = nullptr;
	glm::vec3 textColor = glm::vec3(0, 0, 0);
	glm::vec3 backgroundColor = glm::vec3(1, 1, 1);
	glm::vec3 borderColor = glm::vec3(0, 0, 1);
	glm::vec2 position = glm::vec2(0);
	glm::vec2 dimensions = glm::vec2(128, 36);
	GLfloat scale = 1;
	std::string str = "";
	RenderableRectangle r;
	int focusToken = -1;
	size_t cursorPosition = 0;
};

bool initializeTextInput(TextInput& input);
void updateTextInput(TextInput& input);
void renderTextInput(const TextInput& input, const Shader& shader);