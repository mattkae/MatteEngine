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

	inline bool doesIntersectBox(const Vector2f& p) {
		return p.x >= x && p.x <= (x + w)	&& p.y >= y && p.y <= (y + h);
	}

	inline bool isMouseHovered() {
		return doesIntersectBox(getCursorPosition());
	}

	inline bool isClicked() {
		return isLeftClickDown() && isMouseHovered();
	}

	inline bool isClickedOutside() {
		return isLeftClickDown() && !isMouseHovered();
	}

	void render(const Shader& shader, const Vector4f& backgroundColor, 
		const Vector4f& borderColor = Vector4f{0, 0, 0, 0}, GLfloat borderWidth = 0) const;
};



#endif