#pragma once
#include "Panel.h"
#include "Input.h"
#include "GlobalApplicationState.h"
#include "Rectangle.h"

inline GLfloat getPositioning(UIPositioning positioning, GLfloat absolutePosition, GLfloat windowDimension, GLfloat panelDimension) {
	switch (positioning) {
	case UIPositioning::ABSOLUTE:
		return absolutePosition;
	case UIPositioning::CENTER:
		return (windowDimension - panelDimension) / 2.f;
	case UIPositioning::LEFT: // TOP
		return 0;
	case UIPositioning::RIGHT: // DOWN
		return windowDimension - panelDimension;
	default:
		return 0;
	}
}

void Panel::update() {
	maxWidth = GlobalAppState.floatWidth;
	maxHeight = GlobalAppState.floatHeight;
	GLfloat panelWidth = maxWidth * percentageWidth;
	GLfloat panelHeight = maxHeight * percentageHeight;

	GLfloat x = getPositioning(horizontal, absolutePositioning.x, maxWidth, panelWidth);
	GLfloat y = getPositioning(vertical, absolutePositioning.y, maxHeight, panelHeight);

	boundingRect = { x + padding, y, panelWidth, panelHeight };
}

void Panel::render(const Shader& shader) const {
	boundingRect.render(backgroundColor, borderColor, borderWidth);
}