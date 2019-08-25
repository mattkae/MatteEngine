#pragma once
#include <functional>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "RenderableRectangle.h"
#include "Shader.h"

struct TextRenderer;
class Button {
public:
	Button();
	void initialize();
	void update();
	void render(const Shader& shader) const;

	TextRenderer* textRenderer = nullptr;
	std::function<void(void)> onClickHandler;
	std::string label = "";
	GLfloat padding = 0;
	glm::vec2 position = glm::vec2(0);
	GLfloat scale = 1.0;
	glm::vec3 buttonColor = glm::vec3(0);
	glm::vec3 hoverColor = glm::vec3(0);
	glm::vec3 textColor = glm::vec3(0);
	GLfloat width = 128.f;
private:
	RenderableRectangle mRectangle;
	glm::vec2 mTextPosition = glm::vec2(0);
};