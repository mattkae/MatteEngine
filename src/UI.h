#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include "List.h"
#include <vector>

typedef size_t PanelHandle;
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

	 PanelHandle showModelSelector(Model* models, size_t numModels);
	 PanelHandle showModelPanel(Model& model);
	 PanelHandle showTerrainPanel(Terrain& terrain);

	 void hidePanel(PanelHandle handle);
};