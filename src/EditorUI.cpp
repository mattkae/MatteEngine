#include "EditorUI.h"
#include "Scene.h"
#include "GlobalLoaders.h"
#include "App.h"

void EditorUI::initialize(Scene* scene) {
	ui.init();
	mScene = scene;

	initPrimaryUI(scene);
	initTerrainUI(scene);

	ui.addPanel(&primaryUI);
	ui.addPanel(&terrainUI);
	ui.addPanel(&textureDebuggerUI);
}

void EditorUI::free() {
	ui.free();
}

void onTerrainClick(int index) {
	GlobalApp.editor.editorUI.terrainUI.shouldOpen = true;
}

void EditorUI::initPrimaryUI(Scene* scene) {
	primaryUI.shouldOpen = true;
	primaryUI.isClosable = false;
	primaryUI.panel.percentageHeight = 0.9f;
	primaryUI.panel.percentageWidth = 0.2f;
	primaryUI.panel.vertical = PanelPositioning::PanelPositioning_CENTER;
	primaryUI.panel.horizontal = PanelPositioning::PanelPositioning_LEFT;
	primaryUI.panel.backgroundColor = Vector4f { 0.1f, 0.1f, 0.1f, 0.5f };
	primaryUI.panel.borderColor = Vector4f { 0.5f, 0.5f, 0.5f, 0.5f };
	primaryUI.panel.borderWidth = 2.f;

	UIBuilder::createStandardLabel(&primaryUI, "Terrain");
	UIElement element;
	Button terrainButton;
	terrainButton.label = "Edit Terrain";
	terrainButton.buttonColor = Vector4f { 1.f, 0.f, 0.f, 1.f };
	terrainButton.clickColor = Vector4f { 0.f, 1.f, 0.f, 1.f };
	terrainButton.hoverColor = Vector4f { 0.1f, 0.9f, 0.f, 1.f };
	terrainButton.textColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
	terrainButton.mOnClick = onTerrainClick;
	terrainButton.padding = 2.f;
	element.elementType = UIElementType::BUTTON;
    element.element.button = terrainButton;
	primaryUI.uiElements.add(&element);

	/*Button textureDebugButton;
	textureDebugButton.label = "Debug Textures";
	textureDebugButton.buttonColor = Vector4f { 1.f, 0.f, 0.f, 1.f };
	textureDebugButton.hoverColor = Vector4f { 0.9f, 0.1f, 0.f, 1.f };
	textureDebugButton.textColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
	textureDebugButton.eventType = UIEventType::SHOW_TEXTURE_DEBUGGER;
	textureDebugButton.padding = 2.f;
	element.elementType = UIElementType::BUTTON;
	element.element.button = textureDebugButton;
	primaryUI.uiElements.add(&element);*/

	primaryUI.init();
}

const size_t numVec3 = 3;

void onTerrainSizeChange(void* sizePtr) {
	GlobalApp.scene.mTerrain.mParams.size = *static_cast<int*>(sizePtr);
}

void onGranularityChanged(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.granularity = *static_cast<int*>(ptr);
}

void onTerrainPermChange(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.permSize = *static_cast<int*>(ptr);
}

void onTerrainScaleChange(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.scaleFactor = *static_cast<float*>(ptr);
}

void onTerrainMinMaxChange(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.minMaxHeight = *static_cast<float*>(ptr);
}

void onTerrainFreqChange(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.frequencyFactor = *static_cast<float*>(ptr);
}

void onTerrainAmpChange(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.ampFactor = *static_cast<float*>(ptr);
}

void onTerrainOctaveChange(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.numOctaves = *static_cast<int*>(ptr);
}

void onTerrainNumVerticesChange(void* ptr) {
	GlobalApp.scene.mTerrain.mParams.verticesPerTexture = *static_cast<int*>(ptr);
}

void onTerrainCreate(int data) {

}

void EditorUI::initTerrainUI(Scene* scene)  {
	Terrain* terrain = &scene->mTerrain;
	terrainUI.panel.transitionType = PanelTransitionType_SlideHorizontalNegative;
	terrainUI.panel.percentageHeight = 0.9f;
	terrainUI.panel.percentageWidth = 0.2f;
	terrainUI.panel.vertical = PanelPositioning::PanelPositioning_CENTER;
	terrainUI.panel.horizontal = PanelPositioning::PanelPositioning_RIGHT;
	terrainUI.panel.backgroundColor = Vector4f { 0.1f, 0.1f, 0.1f, 0.5f };
	terrainUI.panel.borderColor = Vector4f { 0.5f, 0.5f, 0.5f, 0.5f };
	terrainUI.panel.borderWidth = 2.f;
	terrainUI.uiElements.allocate(24);
	UIBuilder::createStandardLabel(&terrainUI, "Terrain Editor");
	
	TextInputValue value;

	UIBuilder::createFieldLabel(&terrainUI, "Size");
	value.iVal = terrain->mParams.size;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::INT, onTerrainSizeChange);

	UIBuilder::createFieldLabel(&terrainUI, "Granularity");
	value.iVal = terrain->mParams.granularity;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::INT, onGranularityChanged);

	UIBuilder::createFieldLabel(&terrainUI, "Perm Size");
	value.iVal = terrain->mParams.permSize;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::INT, onTerrainPermChange);

	UIBuilder::createFieldLabel(&terrainUI, "Scale Factor");
	value.fVal = terrain->mParams.scaleFactor;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::FLOAT, onTerrainScaleChange);

	UIBuilder::createFieldLabel(&terrainUI, "Min/Max height");
	value.fVal = terrain->mParams.minMaxHeight;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::FLOAT, onTerrainMinMaxChange);

	UIBuilder::createFieldLabel(&terrainUI, "Amplitude");
	value.fVal = terrain->mParams.ampFactor;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::FLOAT, onTerrainAmpChange);

	UIBuilder::createFieldLabel(&terrainUI, "Frequency");
	value.fVal = terrain->mParams.frequencyFactor;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::FLOAT, onTerrainFreqChange);

	UIBuilder::createFieldLabel(&terrainUI, "Octaves");
	value.iVal = terrain->mParams.numOctaves;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::INT, onTerrainOctaveChange);

	UIBuilder::createFieldLabel(&terrainUI, "Vertices per Texture");
	value.iVal = terrain->mParams.verticesPerTexture;
	UIBuilder::createTextInput(&terrainUI, value, TextInputType::INT, onTerrainNumVerticesChange);

	UIBuilder::createStandardButton(&terrainUI, onTerrainCreate);

	terrainUI.init();
}

void onDebugTexture(int textureIndex) {

}

void EditorUI::initTextureDebuggerUI() {
	if (textureDebuggerUI.isOpen()) {
		return;
	}

	// We free this every time so we get a fresh view of the added textures
	textureDebuggerUI.free();

	textureDebuggerUI.panel.transitionType = PanelTransitionType_SlideHorizontalNegative;
	textureDebuggerUI.panel.percentageHeight = 0.9f;
	textureDebuggerUI.panel.percentageWidth = 0.2f;
	textureDebuggerUI.panel.vertical = PanelPositioning::PanelPositioning_CENTER;
	textureDebuggerUI.panel.horizontal = PanelPositioning::PanelPositioning_RIGHT;
	textureDebuggerUI.panel.backgroundColor = Vector4f { 0.1f, 0.1f, 0.1f, 0.5f };
	textureDebuggerUI.panel.borderColor = Vector4f { 0.5f, 0.5f, 0.5f, 0.5f };
	textureDebuggerUI.panel.borderWidth = 2.f;
	textureDebuggerUI.uiElements.allocate(GlobalTextureLoader.textureList.numElements + 1);
	UIBuilder::createStandardLabel(&textureDebuggerUI, "Texture Debugger");
	for (size_t textureIdx = 0; textureIdx < GlobalTextureLoader.textureList.numElements; textureIdx++) {
		StringBuilder sb;
		UIElement element;
		Button button;
		sb.addInt(GlobalTextureLoader.textureList[textureIdx]);
		button.label = sb.toString();
		button.buttonColor = Vector4f { 0.0f, 0.6f, 0.3f, 1.0f };
		button.hoverColor = Vector4f { 0.0f, 0.9f, 0.1f, 1.0f };
		button.textColor = Vector4f { 1.0f, 1.0f, 1.0f, 1.0f };
		button.data = GlobalTextureLoader.textureList[textureIdx];
		button.padding = 2.f;
		button.mOnClick = onDebugTexture;
		element.elementType = UIElementType::BUTTON;
		element.element.button = button;
		textureDebuggerUI.uiElements.add(&element);
		sb.free();
	}

	textureDebuggerUI.init();
}
