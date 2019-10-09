#include "Rectangle.h"
#include "../Input.h"

struct RenderableRectangle {
	Rectangle r;
	GLuint mVao = 0;
	GLuint mVbo = 0;
};

void initializeRenderableRectangle(RenderableRectangle& rect, bool isDynamic) {
	glGenVertexArrays(1, &rect.mVao);
	glGenBuffers(1, &rect.mVbo);

	GLfloat xStart = rect.r.x;
	GLfloat xEnd = xStart + rect.r.w;
	GLfloat yStart = rect.r.y;
	GLfloat yEnd = yStart + rect.r.h;

	GLfloat vertices[12] = {
		xStart, yStart,
		xStart, yEnd,
		xEnd, yEnd,

		xEnd, yEnd,
		xStart, yStart,
		xEnd, yStart,
	};

	glBindVertexArray(rect.mVao);
	glBindBuffer(GL_ARRAY_BUFFER, rect.mVbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &vertices, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*) 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void renderRenderableRectangle(const RenderableRectangle& rect, const Shader& shader, glm::vec4 backgroundColor) {
	setShaderVec4(shader, "uColor", backgroundColor);
	glBindVertexArray(rect.mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

RenderableRectangle globalRect;

void bufferRect(const Rectangle& rect) {
	GLfloat xStart = rect.x;
	GLfloat xEnd = xStart + rect.w;
	GLfloat yStart = rect.y;
	GLfloat yEnd = yStart + rect.h;
	GLfloat vertices[12] = {
		xStart, yStart,
		xStart, yEnd,
		xEnd, yEnd,

		xEnd, yEnd,
		xStart, yStart,
		xEnd, yStart,
	};

	glBindBuffer(GL_ARRAY_BUFFER, globalRect.mVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(GLfloat), &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderRectangle(const Rectangle& rect, const Shader& shader, glm::vec4 backgroundColor, glm::vec4 borderColor, GLfloat borderWidth) {
	if (globalRect.mVao == 0) {
		initializeRenderableRectangle(globalRect, true);
	}

	Rectangle borderRect;
	borderRect.x = rect.x - borderWidth;
	borderRect.y = rect.y - borderWidth;
	borderRect.h = rect.h + 2.f * borderWidth;
	borderRect.w = rect.w + 2.f * borderWidth;
	setShaderVec4(shader, "uColor", borderColor);
	bufferRect(borderRect);
	glBindVertexArray(globalRect.mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	setShaderVec4(shader, "uColor", backgroundColor);
	bufferRect(rect);
	glBindVertexArray(globalRect.mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}