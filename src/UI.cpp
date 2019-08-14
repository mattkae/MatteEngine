#include "UI.h"
#include "Scene.h"
#include "Constants.h"
#include <glm/glm.hpp>

UI::UI() {
}

void UI::update(double dt) {
	mButton.update();
}

void UI::generate() {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Constants.width), 0.0f, static_cast<float>(Constants.height));
	mOrthographicShader = Shader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	mOrthographicShader.use();
	mOrthographicShader.setMat4("uProjection", projection);

	mTextRenderer.initialize(36, (GLchar*)"C:\\Windows\\Fonts\\Arial.ttf");

	mButton.label = "Hello!";
	mButton.padding = 2;
	mButton.position = glm::vec2(250, 250);
	mButton.buttonColor = glm::vec3(0.5, 0.5, 0.5);
	mButton.textColor = glm::vec3(1.0, 0, 0);
	mButton.textRenderer = &mTextRenderer;
	mButton.onClickHandler = []{ printf("Clicked me!"); };
	mButton.initialize();
}

void UI::render() const {
	mOrthographicShader.use();
	mButton.render(mOrthographicShader);
}