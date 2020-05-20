#include "SceneUIController.h"
#include "Scene.h"

void SceneUIController::selectModel(int modelIdx) {
	if (modelIdx == scene->selectedModelIndex) {
		return;
	}

	Model* model = &scene->models[modelIdx];
	if (modelPanelIdx > -1) {
		scene->ui.hidePanel(modelPanelIdx);
		modelPanelIdx = -1;
	}

	modelPanelIdx = scene->ui.showModelEditor(model);
	scene->selectedModelIndex = modelIdx;
	scene->debugModel.debugBox = scene->modelBoundingBoxes[modelIdx];
}

void SceneUIController::deselectModel() {
	if (scene->selectedModelIndex < 0) {
		return;
	}

	scene->selectedModelIndex = -1;
	scene->ui.hidePanel(modelPanelIdx);
	modelPanelIdx = -1;
}