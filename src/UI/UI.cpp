#include "UI.h"
#include "../Scene.h"
#include "../GlobalApplicationState.h"
#include <glm/glm.hpp>

UI::UI() {
}

void UI::update(double dt) {
	updateUIContext(mContext, mTextRenderer);
}

void UI::generate() {
	mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	mTextRenderer.initialize(16, (GLchar*)"C:\\Windows\\Fonts\\Arial.ttf");

	mPanel.percentageHeight = 0.5f;
	mPanel.percentageWidth = 0.25f;
	mPanel.horizontal = UIPositioning::CENTER;
	mPanel.vertical = UIPositioning::CENTER;
	mPanel.borderColor = glm::vec4(1, 0, 0, 1);
	mPanel.backgroundColor = glm::vec4(0, 0, 1, 1);
	mPanel.borderWidth = 4;
	setPanelPosition(mPanel);

	mButton.label = "Hello!";
	mButton.padding = 2;
	mButton.position = glm::vec2(25, GlobalAppState.floatHeight - 25);
	mButton.buttonColor = glm::vec4(0.5, 0.5, 0.5, 1);
	mButton.textColor = glm::vec4(1.0, 0, 0, 1);
	mButton.onClickHandler = []{ printf("Clicked me!"); };

	mInput.textColor = glm::vec3(1, 0, 0);
	mInput.backgroundColor = glm::vec4(0, 1, 0, 1);
	mInput.focusedBackgroundColor = glm::vec4(0.5, 1, 0, 1);
	mInput.str = "Enter text";
	mInput.bt.padding = 1.f;
	mInput.bt.scale = 1.f;;
	mInput.bt.rect.x = 25;
	mInput.bt.rect.y = GlobalAppState.floatHeight - 100.f;
	mInput.bt.rect.w = 128;
	mInput.bt.rect.h = static_cast<GLfloat>(mTextRenderer.getFontSize());
	initializeTextInput(mInput);

	std::vector<Button> buttons;
	buttons.push_back(mButton);
	buttons.push_back(mButton);
	buttons.push_back(mButton);
	buttons.push_back(mButton);
	mContext.buttons = buttons;
	mContext.panel = mPanel;

	std::vector<TextInput> inputs;
	inputs.push_back(mInput);
	inputs.push_back(mInput);
	inputs.push_back(mInput);
	inputs.push_back(mInput);
	mContext.inputs = inputs;
}

void UI::render() const {
	useShader(mOrthographicShader);
	glm::mat4 projection = glm::ortho(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	setShaderMat4(mOrthographicShader, "uProjection", projection);
	renderUIContext(mContext, mOrthographicShader, mTextRenderer);
}