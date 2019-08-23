#include "UI.h"
#include "Scene.h"
#include "Constants.h"
#include <glm/glm.hpp>

UI::UI() {
}

void UI::update(double dt) {
	mButton.update();
	updateTextInput(mInput);
}

void UI::generate() {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Constants.width), 0.0f, static_cast<float>(Constants.height));
	mOrthographicShader = Shader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	mOrthographicShader.use();
	mOrthographicShader.setMat4("uProjection", projection);

	mTextRenderer.initialize(16, (GLchar*)"C:\\Windows\\Fonts\\Arial.ttf");

	mButton.label = "Hello!";
	mButton.padding = 2;
	mButton.position = glm::vec2(25, Constants.height - 25);
	mButton.buttonColor = glm::vec3(0.5, 0.5, 0.5);
	mButton.textColor = glm::vec3(1.0, 0, 0);
	mButton.textRenderer = &mTextRenderer;
	mButton.onClickHandler = []{ printf("Clicked me!"); };
	mButton.initialize();

	mInput.textRenderer = &mTextRenderer;
	mInput.padding = 2;
	mInput.textColor = glm::vec3(1, 0, 0);
	mInput.backgroundColor = glm::vec3(0, 1, 0);
	mInput.focusedBackgroundColor = glm::vec3(0.5, 1, 0);
	mInput.borderColor = glm::vec3(1, 0, 0);
	mInput.position = glm::vec2(25, Constants.height - 75);
	mInput.str = "Enter text";
	initializeTextInput(mInput);
}

void UI::render() const {
	mOrthographicShader.use();
	renderTextInput(mInput, mOrthographicShader);
	mOrthographicShader.use();
	mButton.render(mOrthographicShader);
}