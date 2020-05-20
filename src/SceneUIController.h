#pragma once

struct Scene;

struct SceneUIController {
	Scene* scene;
	int modelPanelIdx = -1;

	void selectModel(int modelIndex);
	void deselectModel();
};