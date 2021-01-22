#include "UI.h"
#include "../App.h"
#include "../Logger.h"
#include "../Input.h"
#include <GLFW/glfw3.h>

namespace UI {
	String baseString = "Hello World";
	char* FONT_FILE = (char*)"assets/fonts/consola.ttf";

	void PrimaryContext::initialize() {
		uiRenderer.initialize();
		textRenderer.initialize(14, FONT_FILE);
	}

	void PrimaryContext::update(float dt) {
		uiRenderer.flush();
		textRenderer.flush();
		FOREACH(contextList) {
		    value->update(dt, &uiRenderer, &textRenderer, &sb);
		}
	}

	void PrimaryContext::render() const {
		uiRenderer.render();
		textRenderer.render();
	}

	void PrimaryContext::free() {
		uiRenderer.free();
		textRenderer.free();
		FOREACH(contextList) {
			value->free();
		}
		sb.free();
	}

	inline bool isHovering(BatchedBox* box) {
		Vector2f p = getCursorPosition();
		return p.x >= box->position.x && p.x <= (box->position.x + box->size.x) 
			&& p.y >= box->position.y && p.y <= (box->position.y + box->size.y);
	}

	enum ContextTransition {
		ContextTransition_NONE,
		ContextTransition_HOVER,
		ContextTransition_UNHOVER,
		ContextTransition_CLICK,
		ContextTransition_UNCLICK
	};

	void Context::update(float dt, UIRenderer* uiRenderer, TextRenderer* textRenderer, StringBuilder* sb) {
		for (int idx = 0; idx < 2; idx++) {
			DimensionAttribute& dimensionAttr = attr.dimensionAttrList[idx];

			// Sizing
			switch (dimensionAttr.sizing) {
			case Sizing_STATIC: {
				dimension[idx] = dimensionAttr.fixedSize;
				break;
			}
			case Sizing_DYNAMIC: {
				float wholeSize = 0;
				if (parent != NULL) {
					wholeSize = parent->dimension[idx];
				} else {
					wholeSize = idx == 0 ? GlobalApp.floatWidth : GlobalApp.floatHeight;
				}
				
				dimension[idx] = dimensionAttr.percentageSize * wholeSize;
				break;
			}
			}

			// Positioning
			switch (dimensionAttr.positioning) {
			case Positioning_ABSOLUTE: {
				position[idx] = dimensionAttr.position;
				break;
			}
			case Positioning_MIN: {
				if (parent != NULL) {
					position[idx] = parent->position[idx];
				} else {
					position[idx] = 0;
				}
				break;
			}
			case Positioning_MAX: {
				if (parent != NULL) {
					position[idx] = parent->position[idx] + parent->dimension[idx] - dimension[idx];
				} else {
					position[idx] = (idx == 0 ? GlobalApp.floatWidth : GlobalApp.floatHeight) - dimension[idx];
				}
				break;
			}
			case Positioning_CENTER: {
				if (parent != NULL) {
					position[idx] = parent->position[idx] + (parent->dimension[idx] - dimension[idx]) / 2.f;
				} else {
					position[idx] = ((idx == 0 ? GlobalApp.floatWidth : GlobalApp.floatHeight) - dimension[idx]) / 2.f;
				}
				break;
			}
			case Positioning_FLOW: {
				// @TODO: This should be fairly easy
			    if (parent == NULL) {
					logger_warning("Top-level element cannot have flow");
					break;
				}

				switch (parent->attr.flowType) {
				case FlowType_IN_FLOW: {
					Context* relativeContext = nullptr;
					if (prevNeighbor == nullptr) {
						relativeContext = parent;
					} else {
						relativeContext = prevNeighbor;
					}

					if (idx == 0) {
						position[idx] = relativeContext->position[idx] + relativeContext->dimension[idx] + attr.padding;
					} else {
						GLfloat yStart = 0;
						if (relativeContext == parent) {
							yStart = (relativeContext->position[idx] + relativeContext->dimension[idx]);
						} else {
							yStart = relativeContext->position[idx];
						}

						position[idx] = yStart - dimension[idx] - attr.padding;
					}
					break;
				}
				}
			}
			}
		}

		box.position = position;
		box.color = attr.backgroundColor;
		box.size = dimension;
		uiRenderer->add(&box);

		BatchedText batchedText ={
			position,
			Vector4f { position.x,  position.x + dimension.x, position.y, position.y + dimension.y },
			attr.textColor,
			&content
		};

		for (int idx = 0; idx < 2; idx++) {
			DimensionAttribute& dimensionAttr = attr.dimensionAttrList[idx];

			switch (dimensionAttr.alignment) {
			case Alignment_CENTER: {
				float strSize = idx == 0 ? textRenderer->getStringWidth(&content, 1.f) : textRenderer->getStringHeight(&content, 1.f);
				batchedText.position[idx] = batchedText.position[idx]  + ((box.size[idx] - strSize) / 2.f);
				break;
			}
			case Alignment_Right: {
				float strSize = idx == 0 ? textRenderer->getStringWidth(&content, 1.f) : textRenderer->getStringHeight(&content, 1.f);
				batchedText.position[idx] = (batchedText.position[idx]  + box.size[idx]) - strSize;
				break;
			}
			}
		}

		textRenderer->add(&batchedText, 1.f);

		ContextTransition transition = ContextTransition_NONE;
		// @TODO: Work on not propagating this all the way
		if (isHovering(&box)) {
			if (!mIsHovered) {
				mIsHovered = true;
				if (attr.onMouseenter != NULL) {
					attr.onMouseenter(this);
				}
				transition = ContextTransition_HOVER;
			}

			if (isLeftClickDown() || (parent != NULL && isLeftClickDown(parent->focusToken))) {
				if (!mIsClicked) {
					mIsClicked = true;
					if (attr.onMousedown != NULL) {
						attr.onMousedown(this);
					}

					transition = ContextTransition_CLICK;

					if (parent != NULL && parent->mHasFocus) {
						parent->mHasFocus = false;
						focusToken = parent->focusToken;
					} else {
						focusToken = grabFocus();
					}

					mHasFocus = true;
				}
			} else if (mIsClicked) {
				mIsClicked = false;
				if (attr.onMouseup != NULL) {
					attr.onMouseup(this);
				}
			}
		} else {
			if (mIsHovered) {
				mIsHovered = false;
				if (attr.onMouseleave != NULL) {
					attr.onMouseleave(this);
				}
				transition = ContextTransition_UNHOVER;
			}

			if (mIsClicked) {
				mIsClicked = false;
				if (attr.onMouseup != NULL) {
					attr.onMouseup(this);
				}
			}

			if (mHasFocus && isLeftClickDown(focusToken)) {
				resetFocus();
				focusToken = -1;
				mHasFocus = false;
			}
		}

		// Update the various elements with their new positions and dimensions
		switch (attr.type) {
		case ElementType_CONTAINER: {
			break;
		}
		case ElementType_INPUT: {
			if (transition == ContextTransition_HOVER) {
				setCursorToText();
			} else if (transition == ContextTransition_UNHOVER) {
				resetCursor();
			} else if (transition == ContextTransition_CLICK) {
				Vector2f cursorPosition = getCursorPosition();
				const char* value = content.getValue();
				float offset = batchedText.position.x;
				int charIdx = 0;
				for (; charIdx < content.length; charIdx++) {
					if (cursorPosition.x < offset) {
						break;
					}
					offset += textRenderer->getCharWidth(value[charIdx], 1.f);
				}
				cursorCharPosition = charIdx;
			}

			if (mHasFocus) {
				BatchedBox cursorBox = {
					Vector2f { batchedText.position.x, batchedText.position. y },
					Vector2f { 2, static_cast<GLfloat>(textRenderer->mSize) },
					Vector4f { 1.f, 0.f, 0.f, 1.f }
				};

				bool isUpperCase = false;
				if (isKeyDown(GLFW_KEY_LEFT_SHIFT, focusToken) || isKeyDown(GLFW_KEY_RIGHT_SHIFT, focusToken)) {
					isUpperCase = true;
				}

				if (isKeyJustDown(GLFW_KEY_RIGHT, focusToken)) {
					cursorCharPosition++;
					if (cursorCharPosition > content.length) {
						cursorCharPosition = content.length;
					}
				} else if (isKeyJustDown(GLFW_KEY_LEFT, focusToken)) {
					cursorCharPosition--;
					if (cursorCharPosition < 0) {
						cursorCharPosition = 0;
					}
				} else if (isKeyJustDown(GLFW_KEY_BACKSPACE, focusToken)) {
					if (cursorCharPosition - 1 >= 0) {
						sb->clear();
						sb->addStr(content.getValue());
						sb->removeAt(cursorCharPosition - 1, 1);
						sb->copyTo(&content);
						cursorCharPosition--;
					}
				} else {
					int keyDown = getCurrentKeyDown(focusToken);
					if (keyDown >= 32 && keyDown < 128) {
						// TODO: This is slow and really memory inefficient, but not sure if I entirely care right now
						if (isKeyJustDown(keyDown, focusToken)) {
							sb->clear();
							sb->addStr(content.getValue());
							sb->insert(isUpperCase ? keyDown : tolower(keyDown), cursorCharPosition);
							sb->copyTo(&content);
							cursorCharPosition++;
						}
					}
				}

				cursorBox.position.x += textRenderer->getStringWidth(&content, 1.f, cursorCharPosition);
				uiRenderer->add(&cursorBox);
			}
			break;
		}
		case ElementType_BUTTON: {
			if (transition == ContextTransition_HOVER) {
				setCursorToPointer();
			} else if (transition == ContextTransition_UNHOVER) {
				resetCursor();
			}
			break;
		}
		}
		
		// Children
		FOREACH(subContext) {
			value->update(dt, uiRenderer, textRenderer, sb);
		}
	}

	void Context::render() const {
		switch (attr.type) {
		case ElementType_CONTAINER: {
			break;
		}
		case ElementType_INPUT: {
			break;
		}
		case ElementType_BUTTON: {
			break;
		}
		}
		
		FOREACH(subContext) {
			value->render();
		}
	}

	void Context::free() {
		content.free();
		FOREACH(subContext) {
			value->free();
		}
		subContext.deallocate();
	}

    void CreateUI(Context* context, CreateElementAttribute attr) {
		context->attr = attr.attr;

		if (attr.defaultContent != NULL) {
			context->content = attr.defaultContent;
		}

		if (attr.subElements.numElements > 0) {
			context->subContext.allocate(attr.subElements.numElements);
			context->subContext.growDynamically = false;

			FOREACH(attr.subElements) {
				Context sub;
				CreateUI(&sub, *value);
				sub.parent = context;
				if (idx > 0) {
					sub.prevNeighbor = &context->subContext[idx - 1];
				}

				if (idx < attr.subElements.numElements - 1) {
					sub.nextNeighbor = &context->subContext[idx + 1];
				}
			
				context->subContext.add(sub);
			}
		}
	}
	
};
