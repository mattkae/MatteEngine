#include "Rectangle.h"
#include "Input.h"
#include "ShaderUniformMapping.h"

struct RenderableRectangle {
	Rectangle r;
	GLuint mVao = 0;
	GLuint mVbo = 0;

	void init(bool isDynamic) {
		glGenVertexArrays(1, &mVao);
		glGenBuffers(1, &mVbo);

		GLfloat xStart = r.x;
		GLfloat xEnd = xStart + r.w;
		GLfloat yStart = r.y;
		GLfloat yEnd = yStart + r.h;

		GLfloat vertices[12] = {
			xStart, yStart,
			xStart, yEnd,
			xEnd, yEnd,

			xEnd, yEnd,
			xStart, yStart,
			xEnd, yStart,
		};

		glBindVertexArray(mVao);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &vertices, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*) 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

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

void Rectangle::render(const Vector4f& backgroundColor, const Vector4f& borderColor, GLfloat borderWidth) const {
	if (globalRect.mVao == 0) {
		globalRect.init(true);
	}

	Rectangle borderRect;
	borderRect.x = x - borderWidth;
	borderRect.y = y - borderWidth;
	borderRect.h = h + 2.f * borderWidth;
	borderRect.w = w + 2.f * borderWidth;
	setShaderVec4(ShaderUniformMapping::GlobalOrthographicShaderMapping.COLOR, borderColor);
	bufferRect(borderRect);
	glBindVertexArray(globalRect.mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	setShaderVec4(ShaderUniformMapping::GlobalOrthographicShaderMapping.COLOR, backgroundColor);
	bufferRect(*this);
	glBindVertexArray(globalRect.mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}