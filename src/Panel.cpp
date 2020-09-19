#pragma once
#include "Panel.h"
#include "Input.h"
#include "App.h"
#include "Rectangle.h"

inline GLfloat getPositioning(PanelPositioning positioning, GLfloat absolutePosition, GLfloat windowDimension, GLfloat panelDimension) {
	switch (positioning) {
	case PanelPositioning::PanelPositioning_ABSOLUTE:
		return absolutePosition;
	case PanelPositioning::PanelPositioning_CENTER:
		return (windowDimension - panelDimension) / 2.f;
	case PanelPositioning::PanelPositioning_LEFT: // TOP
		return 0;
	case PanelPositioning::PanelPositioning_RIGHT: // DOWN
		return windowDimension - panelDimension;
	default:
		return 0;
	}
}

void Panel::show() {
	panelState = PanelState::PanelState_Opening;
	currentPercentOpen = 1.f - currentPercentOpen;
}

void Panel::hide() {
	panelState = PanelState::PanelState_Closing;
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
	GLfloat x = getPositioning(horizontal, absolutePositioning.x, GlobalApp.floatWidth, panelWidth);
	GLfloat y = getPositioning(vertical, absolutePositioning.y, GlobalApp.floatHeight, panelHeight);
	
	if (panelState == PanelState_Closing) {
		switch (transitionType) {
		case PanelTransitionType::PanelTransitionType_None:
			panelState = PanelState::PanelState_Hide;
			break;
		case PanelTransitionType::PanelTransitionType_SlideHorizontalPositive: {
			currentPercentOpen = (currentPercentOpen + dtMs) / openingTimeMs;
			x -= panelWidth * currentPercentOpen;

			if (currentPercentOpen <= 0.f) {
				panelState = PanelState::PanelState_Hide;
				currentPercentOpen = 0.f;
			}
			break;
		}
		case PanelTransitionType::PanelTransitionType_SlideHorizontalNegative: {
			currentPercentOpen = (currentPercentOpen + dtMs) / openingTimeMs;
			x += panelWidth * currentPercentOpen;

			if (currentPercentOpen <= 0.f) {
				panelState = PanelState::PanelState_Hide;
				currentPercentOpen = 0.f;
			}
			break;
		}
		}
	} else if (panelState == PanelState_Opening) {
		switch (transitionType) {
		case PanelTransitionType::PanelTransitionType_None:
			panelState = PanelState::PanelState_Open;
			break;
		case PanelTransitionType::PanelTransitionType_SlideHorizontalPositive: {
			currentPercentOpen = (currentPercentOpen + dtMs) / openingTimeMs;
			x += panelWidth * currentPercentOpen;

			if (currentPercentOpen >= 1.f) {
				panelState = PanelState::PanelState_Open;
				currentPercentOpen = 0.f;
			}
			break;
		}
		case PanelTransitionType::PanelTransitionType_SlideHorizontalNegative: {
			currentPercentOpen = (currentPercentOpen + dtMs) / openingTimeMs;
			x += panelWidth;
			x -= panelWidth * currentPercentOpen;

			if (currentPercentOpen >= 1.f) {
				panelState = PanelState::PanelState_Open;
				currentPercentOpen = 0.f;
			}
			break;
		}
		}
	}

	boundingRect = { x + padding, y, panelWidth, panelHeight };
}

void Panel::render(const Shader& shader) const {
	if (panelState == PanelState::PanelState_Hide) {
		return;
	}

	boundingRect.render(backgroundColor, borderColor, borderWidth);
}