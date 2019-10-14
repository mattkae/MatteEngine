#pragma once
#include "../Shader.h"
#include "Button.h"
#include "../TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include <vector>

// Plan:
// (1) UI Contexts that will have:
//		(a) Panel for position (or Window)
//		(b) Buttons with callbacks
//		(c) Textboxes with callbacks
//		(d) Other UI elements with callbacks
// (2) A list of UI contexts to show

class Scene;
 class UI {
 public:
	 UI();
	 void generate();
	 void update(double dt);
	 void render() const;
	 std::vector<UIContext> contexts;
 private:
	 Shader mOrthographicShader;
	 TextRenderer mTextRenderer;
 };