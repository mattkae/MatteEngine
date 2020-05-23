#include "UIEventProcessor.h"
#include "Scene.h"

void UIEventProcessor::addEvent(UIEvent& event) {
	activeEvent.type = event.type;
	activeEvent.data = event.data;
	processEvent();
}

void UIEventProcessor::hideModelPanel() {
	scene->ui.hidePanel(rightPanelIdx);
	rightPanelIdx = -1;
	scene->selectedModelIndex = -1;
}

void UIEventProcessor::processEvent() {
	switch (activeEvent.type) {
		case UIEventType::NONE:
			return;
		case UIEventType::SHOW_MODEL: {
			int modelIdx = *static_cast<int*>(activeEvent.data);
			if (modelIdx == scene->selectedModelIndex) {
				break;
			}

			Model* model = &scene->models[modelIdx];
			if (rightPanelIdx > -1) {
				scene->ui.hidePanel(rightPanelIdx);
				rightPanelIdx = -1;
			}

			rightPanelIdx = scene->ui.showModelEditor(model);
			scene->selectedModelIndex = modelIdx;
			scene->debugModel.debugBox = scene->modelBoundingBoxes[modelIdx];
			break;
		}
		case UIEventType::HIDE_MODEL: {
			hideModelPanel();
			break;
		}
		case UIEventType::EDIT_TRANSLATION_X: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].translation.x = value;
			break;
		}
		case UIEventType::EDIT_TRANSLATION_Y: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].translation.y = value;
			break;
		}
		case UIEventType::EDIT_TRANSLATION_Z: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].translation.z = value;
			break;
		}
		case UIEventType::EDIT_SCALE_X: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].scale.x = value;
			break;
		}
		case UIEventType::EDIT_SCALE_Y: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].scale.y = value;
			break;
		}
		case UIEventType::EDIT_SCALE_Z: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].scale.z = value;
			break;
		}
		case UIEventType::EDIT_ROTATION_X: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].rotation.x = value;
			break;
		}
		case UIEventType::EDIT_ROTATION_Y: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].rotation.y = value;
			break;
		}
		case UIEventType::EDIT_ROTATION_Z: {
			if (scene->selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			scene->models[scene->selectedModelIndex].rotation.z = value;
			break;
		}
	}

	activeEvent.type = UIEventType::NONE;
	activeEvent.data = nullptr;
}