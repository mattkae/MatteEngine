#include "UI.h"
#include "../Scene.h"
#include "../GlobalApplicationState.h"
#include <glm/glm.hpp>

UI::UI() {
}

void UI::update(double dt) {
	updateButton(mButton);
	updateTextInput(mInput);
}

void UI::generate() {
	glm::mat4 projection = glm::ortho(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	useShader(mOrthographicShader);
	setShaderMat4(mOrthographicShader, "uProjection", projection);

	mTextRenderer.initialize(16, (GLchar*)"C:\\Windows\\Fonts\\Arial.ttf");

	mPanel.percentageHeight = 0.5f;
	mPanel.percentageWidth = 0.25f;
	mPanel.horizontal = UIPositioning::CENTER;
	mPanel.vertical = UIPositioning::CENTER;
	mPanel.borderColor = glm::vec4(1, 0, 0, 1);
	mPanel.backgroundColor = glm::vec4(0, 0, 1, 1);
	mPanel.borderWidth = 4;
	initializePanel(mPanel);

	mButton.label = "Hello!";
	mButton.padding = 2;
	mButton.position = glm::vec2(25, GlobalAppState.floatHeight - 25);
	mButton.buttonColor = glm::vec4(0.5, 0.5, 0.5, 1);
	mButton.textColor = glm::vec4(1.0, 0, 0, 1);
	mButton.onClickHandler = []{ printf("Clicked me!"); };
	initializeButton(mButton, mTextRenderer);

	mInput.textColor = glm::vec3(1, 0, 0);
	mInput.backgroundColor = glm::vec4(0, 1, 0, 1);
	mInput.focusedBackgroundColor = glm::vec4(0.5, 1, 0, 1);
	mInput.str = "Enter text";
	mInput.bt.padding = glm::vec2(1.f);
	mInput.bt.scale = 1.f;;
	mInput.bt.box.rect.x = 25;
	mInput.bt.box.rect.y = GlobalAppState.floatHeight - 100.f;
	mInput.bt.box.rect.w = 128;
	mInput.bt.box.rect.h = static_cast<GLfloat>(mTextRenderer.getFontSize());
	initializeTextInput(mInput);
}

void UI::render() const {
	useShader(mOrthographicShader);
	renderTextInput(mInput, mOrthographicShader, mTextRenderer);
	useShader(mOrthographicShader);
	renderButton(mButton, mOrthographicShader, mTextRenderer);
	useShader(mOrthographicShader);
	renderPanel(mPanel, mOrthographicShader);
}