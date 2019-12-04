#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include "LightweightArray.h"
#include <vector>

struct UI {
	 Shader mOrthographicShader;
	 TextRenderer mTextRenderer;
	 LightweightArray<UIContext> context;
};

void initUI(UI& ui);
void updateUI(UI& ui, double dt);
void renderUI(const UI& ui);
void freeUI(UI& ui);