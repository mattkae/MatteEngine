#pragma once
#include "../Shader.h"
#include "Button.h"
#include "../TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include "../LightweightArray.h"
#include <vector>

class Scene;
 class UI {
 public:
	 UI();
	 void generate();
	 void update(double dt);
	 void render() const;

	 LightweightArray<UIContext> context;
 private:
	 Shader mOrthographicShader;
	 TextRenderer mTextRenderer;
 };