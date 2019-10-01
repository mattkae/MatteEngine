#include "InteractableRectangle.h"
#include "../Input.h"
#include "Point.h"

bool isMouseHovered(const InteractableRectangle& rect) {
	Point point = getCursorPosition();
	return doesIntersectBox(rect.rect, point);
}

void updateRectangle(InteractableRectangle& rect) {
	bool isMouseClicked = isLeftClickDown();

	rect.isHovered = isMouseHovered(rect);
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