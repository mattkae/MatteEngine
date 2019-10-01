#ifndef INTERACTABLE_RECTANGLE_H
#define INTERACTABLE_RECTANGLE_H

#include "Rectangle.h"

struct InteractableRectangle {
	Rectangle rect;
	bool isFocused = false;
	bool isClicked = false;
	bool isJustClicked = false;
	bool isHovered = false;
};

void updateRectangle(InteractableRectangle& rect);

#endif