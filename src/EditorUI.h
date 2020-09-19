#pragma once
#include "UIContext.h"
#include "UI.h"
#include "UIEventProcessor.h"

struct Scene;
struct Model;

struct EditorUI {
	UIContext primaryUI;
	UIContext modelUI;
	UIContext terrainUI;
	UIContext textureDebuggerUI;
	UI ui;

	void initialize(Scene* scene);

	void initPrimaryUI(Scene* scene);
	void initModelUI(Scene* scene);
	void initTerrainUI(Scene* scene);
	void initTextureDebuggerUI();
	void selectModel(const Model& model);

	void free();
};