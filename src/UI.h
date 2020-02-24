#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include <vector>

struct UI {
	 Shader mOrthographicShader = 0;
	 TextRenderer mTextRenderer;
	 UIContext* panels = nullptr;
	 size_t numPanels = 0;

	 int modelOffset = 0;
};

void initUI(UI& ui);
void updateUI(UI& ui, double dt);
void renderUI(const UI& ui);
void freeUI(UI& ui);

void openModelPanel(UI& ui, size_t index);