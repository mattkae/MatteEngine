#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "Rectangle.h"
#include "GlobalApplicationState.h"
#include "Vector2f.h"
#include "Vector4f.h"

enum UIPositioning {
	ABSOLUTE = 0,
	LEFT = 1,
	CENTER = 2,
	RIGHT = 3,
};

struct Panel {
	GLfloat maxWidth = GlobalAppState.floatWidth;
	GLfloat maxHeight = GlobalAppState.floatHeight;

	Rectangle boundingRect = { 0, 0, GlobalAppState.floatWidth, GlobalAppState.floatHeight };
	GLfloat percentageWidth = 0.25f;
	GLfloat percentageHeight = 0.90f;
	UIPositioning horizontal = UIPositioning::ABSOLUTE;
	UIPositioning vertical = UIPositioning::ABSOLUTE;
	Vector2f absolutePositioning;
	Vector4f backgroundColor = Vector4f{1.f, 1.f, 1.f, 1.f};
	Vector4f borderColor = Vector4f{1.f, 1.f, 1.f, 1.f};
	GLfloat borderWidth = 1.f;
	GLfloat padding = 2.f;
	bool isClosable = true;

	void update();
	void render(const Shader& shader) const;
};