#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Input.h"
#include <GL/glew.h>
#include "Vector4f.h"

struct Rectangle {
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat w = 0.0;
	GLfloat h = 0.0;

	inline bool doesIntersectBox(const Vector2f& p) {
		return p.x >= x && p.x <= (x + w)	&& p.y >= y && p.y <= (y + h);
	}

	inline Vector2f getTopRight() {
		return { x + w, y + h };
	}

	inline Vector2f getBottomLeft() {
		return { x, y };
	}

	bool isOverlapping(Rectangle *other);
	Rectangle getOverlap(Rectangle* other);

	inline bool isMouseHovered() {
		return doesIntersectBox(getCursorPosition());
	}

	inline bool isClicked() {
		return isLeftClickDown() && isMouseHovered();
	}

	inline bool isClickedOutside() {
		return isLeftClickDown() && !isMouseHovered();
	}

	void render(const Vector4f& backgroundColor, 
		const Vector4f& borderColor = Vector4f{0, 0, 0, 0}, GLfloat borderWidth = 0) const;

	void renderBorderRect(const Vector4f& borderColor = Vector4f{0, 0, 0, 0}, GLfloat borderWidth = 0) const;
	void renderTexture(GLuint texture, const Vector4f& borderColor = Vector4f{0, 0, 0, 0}, GLfloat borderWidth = 0) const;
};

#endif