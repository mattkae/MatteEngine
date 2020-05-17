#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include "List.h"
#include "Quaternion.h"
#include <vector>

struct Model;
struct Terrain;

struct UI {
	 Shader mOrthographicShader = 0;
	 TextRenderer mTextRenderer;
	 List<UIContext> panels;

	 void init();
	 void update(double dt);
	 void render() const;
	 void free();

	 size_t getNextPanelIdx();
	 size_t showModelSelector(Model* models, size_t numModels);
	 void showModelPanel(void* modelAddress);
	 size_t showTerrainPanel(Terrain& terrain);

	 void hidePanel(size_t index);
};