#include "RenderableRectangle.h"
#include "Shader.h"
#include "Input.h"

void initializeRenderableRectangle(RenderableRectangle& rect) {
	glGenVertexArrays(1, &rect.mVao);
	glGenBuffers(1, &rect.mVbo);
	glGenBuffers(1, &rect.mEbo);

	GLfloat xStart = rect.r.x;
	GLfloat xEnd = xStart + rect.r.w;
	GLfloat yStart = rect.r.y;
	GLfloat yEnd = yStart + rect.r.h;

	GLfloat vertices[8] = {
		xEnd, yEnd,
		xEnd, yStart,
		xStart, yStart,
		xStart, yEnd
	};

	GLint indicies[6] = {
		0, 1, 3, 1, 2, 3
	};

	glBindVertexArray(rect.mVao);
	glBindBuffer(GL_ARRAY_BUFFER, rect.mVbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect.mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*) 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool isMouseHovered(const Input& input, const RenderableRectangle& rect) {
	DoublePoint point = input.getCursorPosition();
	return doesIntersectBox(rect.r, point);
}

void updateRenderableRectangle(RenderableRectangle& rect) {
	const Input& input = Input::getInstance();

	bool isMouseClicked = input.isLeftClickDown();

	rect.isHovered = isMouseHovered(input, rect);
	bool nextIsClicked = isMouseClicked && rect.isHovered;

	if (nextIsClicked) {
		if (!rect.isClicked) {
			rect.isJustClicked = true;
			rect.isClicked = true;
			rect.isFocused = true;
		} else {
			rect.isJustClicked = false;
		}
	} else {
		if (isMouseClicked) {
			rect.isFocused = false;
		}

		rect.isJustClicked = false;
		rect.isClicked = false;
	}
}

void renderRenderableRectangle(const RenderableRectangle& rect, const Shader& shader) {
	shader.setVec3("uColor", rect.backgroundColor);

	glBindVertexArray(rect.mVao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}