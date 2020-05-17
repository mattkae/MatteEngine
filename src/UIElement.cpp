#include "UIElement.h"

void UIElement::free() {
	switch (elementType) {
	case UIElementType::BUTTON:
		element.button.free();
		break;
	case UIElementType::TEXT_INPUT:
		element.textInput.free();
		break;
	case UIElementType::LABEL:
		element.label.free();
		break;
	}
}