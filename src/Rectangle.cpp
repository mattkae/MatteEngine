#include "Rectangle.h"
#include "Input.h"
#include "ShaderUniformMapping.h"
#include "MathHelper.h"

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
