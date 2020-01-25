#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shader.h"
#include "Input.h"
#include <GL/glew.h>
#include "Vector4f.h"

struct Rectangle {
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat w = 0.0;
	GLfloat h = 0.0;
};

inline bool doesIntersectBox(const Rectangle& r, const Vector2f& p) {
	return p.x >= r.x && p.x <= (r.x + r.w)	&& p.y >= r.y && p.y <= (r.y + r.h);
}

inline bool isMouseHovered(const Rectangle& rect) {
	Vector2f point = getCursorPosition();
	return doesIntersectBox(rect, point);
}

inline bool isClicked(const Rectangle& rect) {
	return isLeftClickDown() && isMouseHovered(rect);
}

inline bool isClickedOutside(const Rectangle& rect) {
	return isLeftClickDown() && !isMouseHovered(rect);
}

void renderRectangle(const Rectangle& rect, const Shader& shader, const Vector4f& backgroundColor, 
	const Vector4f& borderColor = Vector4f{0, 0, 0, 0}, GLfloat borderWidth = 0);

#endif