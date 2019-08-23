#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"

class Scene;

 class UI {
 public:
	 UI();
	 void generate();
	 void update(double dt);
	 void render() const;
	 Scene* scene = nullptr;

 private:
	 Shader mOrthographicShader;
	 Button mButton;
	 TextInput mInput;
	 TextRenderer mTextRenderer;
 };