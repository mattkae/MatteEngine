#include "Rectangle.h"
#include "Input.h"
#include "ShaderUniformMapping.h"
#include "MathHelper.h"

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

		GLfloat vertices[24] = {
			// Positions		// Texture coordinates
			xStart, yEnd,		0.f, 1.f,
			xStart, yStart,		0.f, 0.f,
			xEnd, yStart,		1.f, 0.f,

			xStart, yEnd,		0.f, 1.f,
			xEnd, yStart,		1.f, 0.f,
			xEnd, yEnd,			1.f, 1.f
		};

		glBindVertexArray(mVao);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), &vertices, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*) 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)(2 * sizeof(float)));

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
	GLfloat vertices[24] = {
		// Positions		// Texture coordinates
		xStart, yEnd,		0.f, 1.f,
		xStart, yStart,		0.f, 0.f,
		xEnd, yStart,		1.f, 0.f,

		xStart, yEnd,		0.f, 1.f,
		xEnd, yStart,		1.f, 0.f,
		xEnd, yEnd,			1.f, 1.f
	};

	glBindBuffer(GL_ARRAY_BUFFER, globalRect.mVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(GLfloat), &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Rectangle::render(const Vector4f& backgroundColor, const Vector4f& borderColor, GLfloat borderWidth) const {
	if (globalRect.mVao == 0) {
		globalRect.init(true);
	}

	setShaderBool(ShaderUniformMapping::GlobalOrthographicShaderMapping.USE_TEXTURE, false);

	renderBorderRect(borderColor, borderWidth);

	setShaderVec4(ShaderUniformMapping::GlobalOrthographicShaderMapping.COLOR, backgroundColor);
	bufferRect(*this);
	glBindVertexArray(globalRect.mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Rectangle::renderBorderRect(const Vector4f& borderColor, GLfloat borderWidth) const {
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
}

void Rectangle::renderTexture(GLuint texture, const Vector4f& borderColor, GLfloat borderWidth) const {
	if (globalRect.mVao == 0) {
		globalRect.init(true);
	}

	renderBorderRect(borderColor, borderWidth);

	setShaderBool(ShaderUniformMapping::GlobalOrthographicShaderMapping.USE_TEXTURE, true);
	setShaderInt(ShaderUniformMapping::GlobalOrthographicShaderMapping.TEXTURE, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	bufferRect(*this);
	glBindVertexArray(globalRect.mVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

Rectangle Rectangle::getOverlap(Rectangle *other) {
	Rectangle retval;

	GLfloat leftX   = MAX( x, other->x );
	GLfloat rightX  = MIN( x + w, other->x + other->w );
	GLfloat topY	= MIN( y + h, other->y + other->h );
	GLfloat bottomY = MAX( y, other->y );
		
	retval.x = leftX;
	retval.y = bottomY;
	retval.w = rightX - leftX;
	retval.h = topY - bottomY;
		
	return retval;
}

bool Rectangle::isOverlapping(Rectangle *other) {
	Vector2f thisTopRight = getTopRight();
	Vector2f thisBottomLeft = getBottomLeft();
	Vector2f otherTopRight = other->getTopRight();
	Vector2f otherBottomLeft = other->getBottomLeft();
		
	if (thisTopRight.y < otherBottomLeft.y || thisBottomLeft.y > otherTopRight.y) {
	    return false;
	}

	if (thisTopRight.x < otherBottomLeft.x || thisBottomLeft.x > otherTopRight.x) {
	    return false;
	}

	return true;
}