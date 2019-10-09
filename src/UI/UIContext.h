#pragma once
#include "../Shader.h"
#include "Button.h"
#include "../TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "../Shader.h"
#include <vector>

struct UIContext {
	Panel panel;
	std::vector<Button> buttons;
	std::vector<TextInput> inputs;
	GLfloat spaceBetweenElements = 2.f;
};

void updateUIContext(UIContext& context, const TextRenderer& textRenderer);
void renderUIContext(const UIContext& context, const Shader& shader, const TextRenderer& renderer);