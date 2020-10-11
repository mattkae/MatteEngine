#include "EditorUI.h"
#include "Scene.h"
#include "GlobalLoaders.h"

void EditorUI::initialize(Scene* scene) {
	ui.init();

	initPrimaryUI(scene);
	initModelUI(scene);
	initTerrainUI(scene);

	ui.addPanel(&primaryUI);
	ui.addPanel(&modelUI);
	ui.addPanel(&terrainUI);
	ui.addPanel(&textureDebuggerUI);
}

void EditorUI::free() {
	ui.free();
}

void EditorUI::selectModel(const Model& model) {

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

	int numElements = 0;

	if (scene->numModels && scene->models) {
		numElements += scene->numModels + 1;
	}

	numElements += 2;
	numElements += 1;

	primaryUI.uiElements.allocate(numElements);
	UIBuilder::addStandardLabel("Models", primaryUI);
	String modelName;
	modelName = "Model ";
	for (size_t modelIdx = 0; modelIdx < scene->numModels; modelIdx++) {
		String indexStr;
		indexStr.fromInteger(modelIdx + 1);
		UIElement element;
		Button button;
		button.label.append(modelName);
		button.label.append(indexStr);
		button.buttonColor = Vector4f { 1.0f, 0.0f, 0.0f, 1.0f };
		button.hoverColor = Vector4f { 0.9f, 0.1f, 0.0f, 1.0f };
		button.textColor = Vector4f { 1.0f, 1.0f, 1.0f, 1.0f };
		button.eventType = UIEventType::SHOW_MODEL;
		button.data = modelIdx;
		button.padding = 2.f;
		element.elementType = UIElementType::BUTTON;
		element.element.button = button;
		primaryUI.uiElements.add(&element);
		indexStr.deallocate();
	}

	UIBuilder::addStandardLabel("Terrain", primaryUI);
	UIElement element;
	Button terrainButton;
	terrainButton.label = "Edit Terrain";
	terrainButton.buttonColor = Vector4f { 1.f, 0.f, 0.f, 1.f };
	terrainButton.hoverColor = Vector4f { 0.9f, 0.1f, 0.f, 1.f };
	terrainButton.textColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
	terrainButton.eventType = UIEventType::SHOW_TERRAIN;
	terrainButton.padding = 2.f;
	element.elementType = UIElementType::BUTTON;
	element.element.button = terrainButton;
	primaryUI.uiElements.add(&element);

	Button textureDebugButton;
	textureDebugButton.label = "Debug Textures";
	textureDebugButton.buttonColor = Vector4f { 1.f, 0.f, 0.f, 1.f };
	textureDebugButton.hoverColor = Vector4f { 0.9f, 0.1f, 0.f, 1.f };
	textureDebugButton.textColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
	textureDebugButton.eventType = UIEventType::SHOW_TEXTURE_DEBUGGER;
	textureDebugButton.padding = 2.f;
	element.elementType = UIElementType::BUTTON;
	element.element.button = textureDebugButton;
	primaryUI.uiElements.add(&element);

	primaryUI.init();
}

const size_t numVec3 = 3;

void EditorUI::initModelUI(Scene* scene) {
	modelUI.panel.percentageHeight = 0.9f;
	modelUI.panel.percentageWidth = 0.2f;
	modelUI.panel.vertical = PanelPositioning::PanelPositioning_CENTER;
	modelUI.panel.horizontal = PanelPositioning::PanelPositioning_RIGHT;
	modelUI.panel.backgroundColor = Vector4f { 0.1f, 0.1f, 0.1f, 0.5f };
	modelUI.panel.borderColor = Vector4f { 0.5f, 0.5f, 0.5f, 0.5f };
	modelUI.panel.borderWidth = 2.f;

	size_t numElements = 3 * (numVec3 + 1); // Translation, scaling, rotation with labels
	modelUI.uiElements.allocate(numElements);

	{
		UIBuilder::addStandardLabel("Translation", modelUI);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			//textInput.value.fVal = model->translation[tIdx];
			textInput.eventType = static_cast<UIEventType>((int)UIEventType::EDIT_TRANSLATION_X + tIdx);
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			modelUI.uiElements.add(&element);
		}
	}

	{
		UIBuilder::addStandardLabel("Scaling", modelUI);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			//textInput.value.fVal = model->scale[tIdx];
			textInput.eventType = static_cast<UIEventType>((int)UIEventType::EDIT_SCALE_X + tIdx);
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			modelUI.uiElements.add(&element);
		}
	}

	{
		UIBuilder::addStandardLabel("Rotation", modelUI);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			//textInput.value.fVal = model->rotation[tIdx];
			textInput.eventType = static_cast<UIEventType>((int)UIEventType::EDIT_ROTATION_X + tIdx);
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			modelUI.uiElements.add(&element);
		}
	}
	modelUI.init();
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
	UIBuilder::addStandardLabel("Terrain Editor", terrainUI);
	
	TextInputValue value;

	UIBuilder::addFieldLabel("Size", terrainUI);
	value.iVal = terrain->mParams.size;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_SIZE);

	UIBuilder::addFieldLabel("Granularity", terrainUI);
	value.iVal = terrain->mParams.granularity;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_GRANULARITY);

	UIBuilder::addFieldLabel("Perm Size", terrainUI);
	value.iVal = terrain->mParams.permSize;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_PERM);

	UIBuilder::addFieldLabel("Scale Factor", terrainUI);
	value.fVal = terrain->mParams.scaleFactor;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_SCALE);

	UIBuilder::addFieldLabel("Min/Max height", terrainUI);
	value.fVal = terrain->mParams.minMaxHeight;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_MIN_MAX);

	UIBuilder::addFieldLabel("Amplitude", terrainUI);
	value.fVal = terrain->mParams.ampFactor;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_AMP);

	UIBuilder::addFieldLabel("Frequency", terrainUI);
	value.fVal = terrain->mParams.frequencyFactor;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_FREQ);

	UIBuilder::addFieldLabel("Octaves", terrainUI);
	value.iVal = terrain->mParams.numOctaves;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_OCT);

	UIBuilder::addFieldLabel("Vertices per Texture", terrainUI);
	value.iVal = terrain->mParams.verticesPerTexture;
	UIBuilder::addTextInput(terrainUI, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_NUM_VERTICES);

	{
		UIElement element;
		Button button;
		button.label = "Create";
		button.buttonColor = Vector4f { 0.0f, 0.6f, 0.3f, 1.0f };
		button.hoverColor = Vector4f { 0.0f, 0.9f, 0.1f, 1.0f };
		button.textColor = Vector4f { 1.0f, 1.0f, 1.0f, 1.0f };
		button.eventType = UIEventType::EDIT_TERRAIN_APPLY;
		button.padding = 2.f;
		element.elementType = UIElementType::BUTTON;
		element.element.button = button;
		terrainUI.uiElements.add(&element);
	}
	terrainUI.init();
}

void EditorUI::initTextureDebuggerUI() {
	if (textureDebuggerUI.isOpen()) {
		return;
	}

	// We free this every time so we get a fresh view of the added textures
	textureDebuggerUI.free();

	textureDebuggerUI.panel.transitionType = PanelTransitionType_SlideHorizontalNegative;
	textureDebuggerUI.panel.percentageHeight = 0.1f;
	textureDebuggerUI.panel.percentageWidth = 0.2f;
	textureDebuggerUI.panel.vertical = PanelPositioning::PanelPositioning_CENTER;
	textureDebuggerUI.panel.horizontal = PanelPositioning::PanelPositioning_RIGHT;
	textureDebuggerUI.panel.backgroundColor = Vector4f { 0.1f, 0.1f, 0.1f, 0.5f };
	textureDebuggerUI.panel.borderColor = Vector4f { 0.5f, 0.5f, 0.5f, 0.5f };
	textureDebuggerUI.panel.borderWidth = 2.f;
	textureDebuggerUI.uiElements.allocate(GlobalTextureLoader.textureList.numElements + 1);
	UIBuilder::addStandardLabel("Texture Debugger", textureDebuggerUI);
	for (size_t textureIdx = 0; textureIdx < GlobalTextureLoader.textureList.numElements; textureIdx++) {
		UIElement element;
		Button button;
		button.label.fromInteger(GlobalTextureLoader.textureList[textureIdx]);
		button.buttonColor = Vector4f { 0.0f, 0.6f, 0.3f, 1.0f };
		button.hoverColor = Vector4f { 0.0f, 0.9f, 0.1f, 1.0f };
		button.textColor = Vector4f { 1.0f, 1.0f, 1.0f, 1.0f };
		button.eventType = UIEventType::DEBUG_TEXTURE;
		button.data = GlobalTextureLoader.textureList[textureIdx];
		button.padding = 2.f;
		element.elementType = UIElementType::BUTTON;
		element.element.button = button;
		textureDebuggerUI.uiElements.add(&element);
	}

	textureDebuggerUI.init();
}
