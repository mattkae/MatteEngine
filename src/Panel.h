#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "Rectangle.h"
#include "Vector2f.h"
#include "Vector4f.h"

enum PanelPositioning {
	PanelPositioning_ABSOLUTE = 0,
	PanelPositioning_LEFT = 1,
	PanelPositioning_CENTER = 2,
	PanelPositioning_RIGHT = 3,
};

enum PanelTransitionType {
	PanelTransitionType_None = 0,
	PanelTransitionType_SlideHorizontalPositive = 1,
	PanelTransitionType_SlideHorizontalNegative = 2
};

enum PanelState {
	PanelState_Hide = 0,
	PanelState_Opening = 1,
	PanelState_Open = 2,
	PanelState_Closing = 3
};

struct Panel {
	Rectangle boundingRect = { 0, 0, 0, 0 };

	// Display variables
	GLfloat percentageWidth = 0.25f;
	GLfloat percentageHeight = 0.90f;
	PanelPositioning horizontal = PanelPositioning::PanelPositioning_ABSOLUTE;
	PanelPositioning vertical = PanelPositioning::PanelPositioning_ABSOLUTE;
	PanelTransitionType transitionType = PanelTransitionType::PanelTransitionType_None;
	Vector2f absolutePositioning;
	Vector4f backgroundColor = Vector4f{1.f, 1.f, 1.f, 1.f};
	Vector4f borderColor = Vector4f{1.f, 1.f, 1.f, 1.f};
	GLfloat borderWidth = 1.f;
	GLfloat padding = 2.f;

	// Opening closing variables
	PanelState panelState = PanelState::PanelState_Hide;
	GLfloat currentPercentOpen = 1.f;
	GLfloat openingTimeMs = 1.f;

	void show();
	void hide();
	void update(float dtMs);
	void render(const Shader& shader) const;
};