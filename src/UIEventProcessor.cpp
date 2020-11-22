#include "UIEventProcessor.h"
#include "App.h"

void UIEventProcessor::processEvent(UIEvent& activeEvent) {
	switch (activeEvent.type) {
		case UIEventType::NONE:
			return;
		case UIEventType::CLOSE_PANEL: {
			GlobalApp.editor.editorUI.ui.panels[*static_cast<int*>(activeEvent.data)]->shouldClose = true;
			resetFocus();
			break;
		}
		case UIEventType::SHOW_MODEL: {
			int modelIdx = *static_cast<int*>(activeEvent.data);
			if (modelIdx >= 0 && modelIdx < GlobalApp.scene.numModels) {
				Model* model = &GlobalApp.scene.models[modelIdx];
				GlobalApp.editor.editorUI.openModelUI(model);
			}
			break;
		}
		case UIEventType::HIDE_MODEL: {
			GlobalApp.editor.editorUI.modelUI.shouldClose = true;
			break;
		}
		case UIEventType::EDIT_TRANSLATION_X: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].translation.x = value;
			break;
		}
		case UIEventType::EDIT_TRANSLATION_Y: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].translation.y = value;
			break;
		}
		case UIEventType::EDIT_TRANSLATION_Z: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].translation.z = value;
			break;
		}
		case UIEventType::EDIT_SCALE_X: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].scale.x = value;
			break;
		}
		case UIEventType::EDIT_SCALE_Y: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].scale.y = value;
			break;
		}
		case UIEventType::EDIT_SCALE_Z: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].scale.z = value;
			break;
		}
		case UIEventType::EDIT_ROTATION_X: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].rotation.x = value;
			break;
		}
		case UIEventType::EDIT_ROTATION_Y: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].rotation.y = value;
			break;
		}
		case UIEventType::EDIT_ROTATION_Z: {
			if (GlobalApp.scene.selectedModelIndex < 0) {
				break;
			}

			float value = *static_cast<float*>(activeEvent.data);
			GlobalApp.scene.models[GlobalApp.scene.selectedModelIndex].rotation.z = value;
			break;
		}
		case UIEventType::SHOW_TERRAIN: {
			GlobalApp.editor.editorUI.terrainUI.shouldOpen = true;
			break;
		}
		case UIEventType::EDIT_TERRAIN_SIZE: {
			GlobalApp.scene.mTerrain.mParams.size = *static_cast<int*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_GRANULARITY: {
			GlobalApp.scene.mTerrain.mParams.granularity = *static_cast<int*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_PERM: {
			GlobalApp.scene.mTerrain.mParams.permSize = *static_cast<int*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_SCALE: {
			GlobalApp.scene.mTerrain.mParams.scaleFactor = *static_cast<float*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_MIN_MAX: {
			GlobalApp.scene.mTerrain.mParams.minMaxHeight = *static_cast<float*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_AMP: {
			GlobalApp.scene.mTerrain.mParams.ampFactor = *static_cast<float*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_FREQ: {
			GlobalApp.scene.mTerrain.mParams.frequencyFactor = *static_cast<float*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_OCT: {
			GlobalApp.scene.mTerrain.mParams.numOctaves = *static_cast<int*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_NUM_VERTICES: {
			GlobalApp.scene.mTerrain.mParams.verticesPerTexture = *static_cast<int*>(activeEvent.data);
			break;
		}
		case UIEventType::EDIT_TERRAIN_APPLY: {
			GlobalApp.scene.mTerrain.initialize(GlobalApp.scene.mTerrain.mParams);
			break;
		}
		case UIEventType::SHOW_TEXTURE_DEBUGGER: {
			GlobalApp.editor.editorUI.initTextureDebuggerUI();
			GlobalApp.editor.editorUI.textureDebuggerUI.shouldOpen = true;
			break;
		}
		case UIEventType::DEBUG_TEXTURE: {
			GlobalApp.editor.editorUI.textureDebuggerUI.textureDebugger.show = true;
			GlobalApp.editor.editorUI.textureDebuggerUI.textureDebugger.texture = *static_cast<int*>(activeEvent.data);
			break;
		}
		case UIEventType::SHOW_LIGHT_EDITOR: {
			int lightIdx = *static_cast<int*>(activeEvent.data);
			if (lightIdx >= 0 && lightIdx < GlobalApp.scene.numLightsUsed) {
				Light* light = &GlobalApp.scene.lights[lightIdx];
				GlobalApp.editor.editorUI.openLightUI(light);
			}
			break;
		}
	}

	activeEvent.type = UIEventType::NONE;
	activeEvent.data = nullptr;
}