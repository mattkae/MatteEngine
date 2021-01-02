#include "Panel.h"
#include "Input.h"
#include "App.h"
#include "Rectangle.h"

inline GLfloat getPositioning(PanelPositioning positioning, GLfloat absolutePosition, GLfloat windowDimension, GLfloat panelDimension, GLfloat padding) {
	switch (positioning) {
	case PanelPositioning::PanelPositioning_ABSOLUTE:
		return absolutePosition;
	case PanelPositioning::PanelPositioning_CENTER:
		return (windowDimension - panelDimension) / 2.f;
	case PanelPositioning::PanelPositioning_LEFT: // TOP
		return padding;
	case PanelPositioning::PanelPositioning_RIGHT: // DOWN
		return windowDimension - panelDimension - padding;
	default:
		return 0;
	}
}

void Panel::show() {
	panelState = PanelState::PanelState_Opening;
	currentTransitionTimeMs = 0.f;
}

void Panel::hide() {
	panelState = PanelState::PanelState_Closing;
	currentTransitionTimeMs = 0.f;
}

/**
 * Update the position of the panel in the world based off its positioning attribute.
 * Also perform any animation necessary for opening and closing.
*/
void Panel::update(float dtMs) {
	if (panelState == PanelState::PanelState_Hide) {
		return;
	}

	GLfloat panelWidth = GlobalApp.floatWidth * percentageWidth;
	GLfloat panelHeight = GlobalApp.floatHeight * percentageHeight;
	GLfloat x = getPositioning(horizontal, absolutePositioning.x, GlobalApp.floatWidth, panelWidth, padding);
	GLfloat y = getPositioning(vertical, absolutePositioning.y, GlobalApp.floatHeight, panelHeight, padding);
	
	if (panelState == PanelState_Closing) {
		currentTransitionTimeMs += dtMs;
		currentPercentOpen = currentTransitionTimeMs / openingTimeMs;

		switch (transitionType) {
		case PanelTransitionType::PanelTransitionType_None:
			currentPercentOpen = 0.f;
			break;
		case PanelTransitionType::PanelTransitionType_SlideHorizontalPositive:
			x -= panelWidth * currentPercentOpen;
			break;
		case PanelTransitionType::PanelTransitionType_SlideHorizontalNegative:
			x += panelWidth * currentPercentOpen;
			break;
		}

		if (currentPercentOpen <= 0.f) {
			panelState = PanelState::PanelState_Hide;
			currentPercentOpen = 0.f;
		}
	} else if (panelState == PanelState_Opening) {
		currentTransitionTimeMs += dtMs;
		currentPercentOpen = currentTransitionTimeMs / openingTimeMs;
		switch (transitionType) {
		case PanelTransitionType::PanelTransitionType_None:
			currentPercentOpen = 1.f;
			break;
		case PanelTransitionType::PanelTransitionType_SlideHorizontalPositive:
			x += panelWidth * currentPercentOpen;
			break;
		case PanelTransitionType::PanelTransitionType_SlideHorizontalNegative:
			x += panelWidth;
			x -= panelWidth * currentPercentOpen;
		}

		if (currentPercentOpen >= 1.f) {
			panelState = PanelState::PanelState_Open;
			currentPercentOpen = 1.f;
		}
	}

	boundingRect = { x, y, panelWidth, panelHeight };
}

void Panel::render(const Shader& shader) const {
	if (panelState == PanelState::PanelState_Hide) {
		return;
	}

	boundingRect.render(backgroundColor, borderColor, borderWidth);
}
