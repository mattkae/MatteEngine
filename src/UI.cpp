#include "UI.h"
#include "Scene.h"
#include "GlobalApplicationState.h"
#include "Model.h"
#include "UIEventProcessor.h"

void UI::init() {
	mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	mTextRenderer.initialize(16, (GLchar*)"assets/fonts/consola.ttf");
	panels.allocate(8);
}

void UI::update(double dt) {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		UIContext& panel = panels[idx];
		if (panel.shouldOpen) {
			for (size_t otherIdx = 0; otherIdx < panels.numElements; otherIdx++) {
				// This only runs once every time you open a new context, so we can afford to be slow but robust
				if (panels[otherIdx].isActive
					&& panels[otherIdx].panel.vertical == panel.panel.vertical
					&& panels[otherIdx].panel.horizontal == panel.panel.horizontal) {
					panels[otherIdx].isActive = false;
				}
			}
		}

		panel.update(mTextRenderer, eventProcessor->activeEvent);
	}
}


void UI::render() const {
	useShader(mOrthographicShader);
	Matrix4x4f projection = getOrthographicProjection(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	setShaderMat4(mOrthographicShader, "uProjection", projection);

	for (size_t idx = 0; idx < panels.numElements; idx++) {
		panels[idx].render(mOrthographicShader, mTextRenderer);
	}
}

void UI::free() {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		UIContext* context = panels.getValue(idx);
		context->free();
	}

	panels.deallocate();

	if (mOrthographicShader > 0) {
		glDeleteProgram(mOrthographicShader);
	}
}

size_t UI::getNextPanelIdx() {
	size_t panelIdx = 0;
	for (; panelIdx < panels.numElements; panelIdx++) {
		if (!panels[panelIdx].isActive) {
			break;
		}
	}

	if (panelIdx == panels.numElements) {
		panels.add(new UIContext());
	}

	return panelIdx;
}

size_t UI::showPanel(UIContext* context, int index) {
	size_t panelIdx = index < 0 ? getNextPanelIdx() : index;
	panels.set(context, panelIdx);
	return panelIdx;
}

void addStandardLabel(const char* str, UIContext& context) {
	UIElement labelElement;
	Label label;
	label.bt.padding = 2.f;
	label.backgroundColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
	label.textColor = Vector4f { 0.f, 0.f, 0.f , 1.f };
	label.text = str;
	labelElement.elementType = UIElementType::LABEL;
	labelElement.element.label = label;
	context.uiElements.add(&labelElement);
}

void addFieldLabel(const char* str, UIContext& context) {
	UIElement labelElement;
	Label label;
	label.bt.padding = 2.f;
	label.backgroundColor = Vector4f { 0.1f, 0.1f, 0.1f, 1.f };
	label.textColor = Vector4f { 1.f, 1.f, 1.f , 1.f };
	label.text = str;
	label.bt.scale = 0.75f;
	labelElement.elementType = UIElementType::LABEL;
	labelElement.element.label = label;
	context.uiElements.add(&labelElement);
}

size_t UI::showGlobalSelector(Scene& scene) {
	size_t panelIdx = getNextPanelIdx();
	UIContext& context = panels[panelIdx];
	context.shouldOpen = true;
	context.panel.percentageHeight = 0.9f;
	context.panel.percentageWidth = 0.2f;
	context.panel.vertical = UIPositioning::CENTER;
	context.panel.horizontal = UIPositioning::LEFT;
	context.panel.backgroundColor = Vector4f { 0.1, 0.1, 0.1, 0.5 };
	context.panel.borderColor = Vector4f { 0.5, 0.5, 0.5, 0.5 };
	context.panel.borderWidth = 2.f;

	int numElements = 0;

	if (scene.numModels && scene.models) {
		numElements += scene.numModels + 1;
	}

	if (scene.mTerrain.isInitialized) {
		numElements += 2;
	}

	context.uiElements.allocate(numElements);
	addStandardLabel("Models", context);
	String modelName;
	modelName = "Model ";
	for (size_t modelIdx = 0; modelIdx < scene.numModels; modelIdx++) {
		String indexStr;
		indexStr.fromInteger(modelIdx + 1);
		UIElement element;
		Button button;
		button.label.append(modelName);
		button.label.append(indexStr);
		button.buttonColor = Vector4f { 1.0, 0.0, 0.0, 1.0 };
		button.hoverColor = Vector4f { 0.9, 0.1, 0.0, 1.0 };
		button.textColor = Vector4f { 1.0, 1.0, 1.0, 1.0 };
		button.eventType = UIEventType::SHOW_MODEL;
		button.data = modelIdx;
		button.padding = 2.f;
		element.elementType = UIElementType::BUTTON;
		element.element.button = button;
		context.uiElements.add(&element);
		indexStr.deallocate();
	}

	addStandardLabel("Terrain", context);
	UIElement element;
	Button button;
	button.label = "Edit Terrain";
	button.buttonColor = Vector4f { 1.0, 0.0, 0.0, 1.0 };
	button.hoverColor = Vector4f { 0.9, 0.1, 0.0, 1.0 };
	button.textColor = Vector4f { 1.0, 1.0, 1.0, 1.0 };
	button.eventType = UIEventType::SHOW_TERRAIN;
	button.padding = 2.f;
	element.elementType = UIElementType::BUTTON;
	element.element.button = button;
	context.uiElements.add(&element);

	return panelIdx;
}

const size_t numVec3 = 3;

size_t UI::showModelEditor(Model* model) {
	size_t panelIdx = getNextPanelIdx();
	UIContext& context = panels[panelIdx];
	context.shouldOpen = true;
	context.panel.percentageHeight = 0.9f;
	context.panel.percentageWidth = 0.2f;
	context.panel.vertical = UIPositioning::CENTER;
	context.panel.horizontal = UIPositioning::RIGHT;
	context.panel.backgroundColor = Vector4f { 0.1, 0.1, 0.1, 0.5 };
	context.panel.borderColor = Vector4f { 0.5, 0.5, 0.5, 0.5 };
	context.panel.borderWidth = 2.f;

	size_t numElements = 3 * (numVec3 + 1); // Translation, scaling, rotation with labels
	context.uiElements.allocate(numElements);

	{
		addStandardLabel("Translation", context);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			textInput.value.fVal = model->translation[tIdx];
			textInput.eventType = static_cast<UIEventType>((int)UIEventType::EDIT_TRANSLATION_X + tIdx);
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			context.uiElements.add(&element);
		}
	}

	{
		addStandardLabel("Scaling", context);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			textInput.value.fVal = model->scale[tIdx];
			textInput.eventType = static_cast<UIEventType>((int)UIEventType::EDIT_SCALE_X + tIdx);
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			context.uiElements.add(&element);
		}
	}

	{
		addStandardLabel("Rotation", context);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			textInput.value.fVal = model->rotation[tIdx];
			textInput.eventType = static_cast<UIEventType>((int)UIEventType::EDIT_ROTATION_X + tIdx);
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			context.uiElements.add(&element);
		}
	}

	return panelIdx;
}

void addTextInput(UIContext& context, 
	TextInputValue value, 
	TextInputType inputType,
	UIEventType eventType) 
{
	UIElement element;
	UIElement labelElement;
	TextInput textInput;
	textInput.bt.padding = 2.f;
	textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
	textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
	textInput.inputType = inputType;
	textInput.textColor = Vector4f { 1, 1, 1, 1 };
	textInput.value = value;
	textInput.eventType = eventType;
	element.elementType = UIElementType::TEXT_INPUT;
	element.element.textInput = textInput;
	context.uiElements.add(&element);
}

size_t UI::showTerrainEditor(Terrain* terrain)  {
	size_t panelIdx = getNextPanelIdx();
	UIContext& context = panels[panelIdx];
	context.shouldOpen = true;
	context.panel.percentageHeight = 0.9f;
	context.panel.percentageWidth = 0.2f;
	context.panel.vertical = UIPositioning::CENTER;
	context.panel.horizontal = UIPositioning::RIGHT;
	context.panel.backgroundColor = Vector4f { 0.1, 0.1, 0.1, 0.5 };
	context.panel.borderColor = Vector4f { 0.5, 0.5, 0.5, 0.5 };
	context.panel.borderWidth = 2.f;
	context.uiElements.allocate(18);
	addStandardLabel("Terrain Editor", context);
	
	TextInputValue value;

	addFieldLabel("Size", context);
	value.iVal = terrain->mParams.size;
	addTextInput(context, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_SIZE);

	addFieldLabel("Granularity", context);
	value.iVal = terrain->mParams.granularity;
	addTextInput(context, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_GRANULARITY);

	addFieldLabel("Perm Size", context);
	value.iVal = terrain->mParams.permSize;
	addTextInput(context, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_PERM);

	addFieldLabel("Scale Factor", context);
	value.fVal = terrain->mParams.scaleFactor;
	addTextInput(context, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_SCALE);

	addFieldLabel("Min/Max height", context);
	value.fVal = terrain->mParams.minMaxHeight;
	addTextInput(context, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_MIN_MAX);

	addFieldLabel("Amplitude", context);
	value.fVal = terrain->mParams.ampFactor;
	addTextInput(context, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_AMP);

	addFieldLabel("Frequency", context);
	value.fVal = terrain->mParams.frequencyFactor;
	addTextInput(context, value, TextInputType::FLOAT, UIEventType::EDIT_TERRAIN_FREQ);

	addFieldLabel("Octaves", context);
	value.iVal = terrain->mParams.numOctaves;
	addTextInput(context, value, TextInputType::INT, UIEventType::EDIT_TERRAIN_OCT);

	{
		UIElement element;
		Button button;
		button.label = "Create";
		button.buttonColor = Vector4f { 0.0, 0.6, 0.3, 1.0 };
		button.hoverColor = Vector4f { 0.0, 0.9, 0.1, 1.0 };
		button.textColor = Vector4f { 1.0, 1.0, 1.0, 1.0 };
		button.eventType = UIEventType::EDIT_TERRAIN_APPLY;
		button.padding = 2.f;
		element.elementType = UIElementType::BUTTON;
		element.element.button = button;
		context.uiElements.add(&element);
	}

	return panelIdx;
}

void UI::hidePanel(size_t index) {
	if (index >= panels.numElements) {
		return;
	}

	panels[index].isActive = false;
}