#include "UIContext.h"
#include "../GlobalApplicationState.h"

void updateUIContext(UIContext& context, const TextRenderer& textRenderer) {
	setPanelPosition(context.panel);
	GLfloat yOffset = context.panel.boundingRect.y;
	GLfloat xPosition = context.panel.boundingRect.x + context.panel.padding;

	for (Button &button : context.buttons) {
		button.position = glm::vec2(xPosition, yOffset);
		button.width = GlobalAppState.floatWidth * context.panel.percentageWidth - 2 * button.padding;
		updateButton(button, textRenderer);
		yOffset += getButtonHeight(button, textRenderer);
		yOffset += context.spaceBetweenElements;
	}

	for (TextInput &input: context.inputs) {
		input.bt.rect.x = xPosition;
		input.bt.rect.y = yOffset;
		input.bt.rect.w = GlobalAppState.floatWidth * context.panel.percentageWidth - 2 * input.bt.padding;
		updateTextInput(input);
		yOffset += getTextInputHeight(input, textRenderer);
		yOffset += context.spaceBetweenElements;
	}
}

void renderUIContext(const UIContext& context, const Shader& shader, const TextRenderer& textRenderer) {
	renderPanel(context.panel, shader);

	for (const Button &button : context.buttons) {
		renderButton(button, shader, textRenderer);
	}

	for (const TextInput &input: context.inputs) {
		renderTextInput(input, shader, textRenderer);
	}
}