#include "UI.h"
#include "Scene.h"
#include "App.h"
#include "Model.h"
#include "UIEventProcessor.h"
#include "ShaderUniformMapping.h"

void UI::init() {
	mTextRenderer.initialize(16, (GLchar*)"assets/fonts/consola.ttf");
	panels.allocate(8);
}

void UI::update(double dt) {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		UIContext* panel = panels[idx];
		if (panel->shouldOpen) {
			for (size_t otherIdx = 0; otherIdx < panels.numElements; otherIdx++) {
				// Another panel is open and in it's space
				if (otherIdx != idx 
					&& panels[otherIdx]->isOpen()
					&& panels[otherIdx]->panel.vertical == panel->panel.vertical
					&& panels[otherIdx]->panel.horizontal == panel->panel.horizontal) {
					panels[otherIdx]->shouldClose = true;
				}
			}
		}

		panel->update(static_cast<float>(dt), mTextRenderer, &eventProcessor);
	}
}


void UI::render() const {
	useShader(ShaderUniformMapping::GlobalOrthographicShaderMapping.shader);
	Matrix4x4f projection = getOrthographicProjection(0.0f, GlobalApp.floatWidth, 0.0f, GlobalApp.floatHeight);
	setShaderMat4(ShaderUniformMapping::GlobalOrthographicShaderMapping.PROJECTION, projection);

	for (size_t idx = 0; idx < panels.numElements; idx++) {
		panels[idx]->render(ShaderUniformMapping::GlobalOrthographicShaderMapping.shader, mTextRenderer);
	}

	textureDebugger.render();
}

void UI::free() {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		UIContext* context = *panels.getValue(idx);
		context->free();
	}

	panels.deallocate();
}

void UI::addPanel(UIContext* panel) {
	panels.add(&panel);
}

void UIBuilder::addStandardLabel(const char* str, UIContext& context) {
	UIElement labelElement;
	Label label;
	label.build(2.f, { 1.f, 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f , 1.f });
	label.text = str;
	labelElement.elementType = UIElementType::LABEL;
	labelElement.element.label = label;
	context.uiElements.add(&labelElement);
}

void UIBuilder::addFieldLabel(const char* str, UIContext& context) {
	UIElement labelElement;
	Label label;
	label.build(2.f, { 0.1f, 0.1f, 0.1f, 1.f }, { 1.f, 1.f, 1.f , 1.f }, 0.75f);
	label.text = str;
	label.bt.scale = 0.75f;
	labelElement.elementType = UIElementType::LABEL;
	labelElement.element.label = label;
	context.uiElements.add(&labelElement);
}

void UIBuilder::addTextInput(UIContext& context, 
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
	if (inputType == TextInputType::STRING) {
		textInput.representation.allocate(32);
	}
	element.elementType = UIElementType::TEXT_INPUT;
	element.element.textInput = textInput;


	context.uiElements.add(&element);
}

void TextureDebugScreen::render() const {
	if (!show) {
		return;
	}

	rectangle.renderTexture(texture, { 0.1f, 0.1f, 0.1f, 1.0 }, 2.f);
}