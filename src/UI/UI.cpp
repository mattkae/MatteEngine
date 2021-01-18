#include "UI.h"
#include "../App.h"
#include "../Logger.h"

namespace UI {
	String baseString = "Hello World";
	char* FONT_FILE = (char*)"assets/fonts/consola.ttf";

	void PrimaryContext::initialize() {
		uiRenderer.initialize();
		textRenderer.initialize(36, FONT_FILE);
	}

	void PrimaryContext::update(float dt) {
		uiRenderer.flush();
		textRenderer.flush();
	    BatchedText text = {
			true,
			Vector2f { 500, 500 },
			Vector2f { 0, 0 },
			14,
			Vector4f { 1, 1, 1, 1 },
		    &baseString
		};
		textRenderer.add(&text, 1);
		FOREACH(contextList) {
		    value->update(dt, &uiRenderer);
		}
	}

	void PrimaryContext::render() const {
		uiRenderer.render();
		textRenderer.render();
	}

	void PrimaryContext::free() {
		//uiRenderer.free();
		textRenderer.free();
		FOREACH(contextList) {
			value->free();
		}
	}

	void Context::update(float dt, UIRenderer* uiRenderer) {
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
					position[idx] = ((parent->position[idx] + parent->dimension[idx]) - dimension[idx]) / 2.f;
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
				case FlowType_VERTICAL: {
					break;
				}
				case FlowType_HORIZONTAL: {
					break;
				}
				}
			}
			}
		}

		// Update the various elements with their new positions and dimensions
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

		box.position = position;
		box.color = attr.backgroundColor;
		box.size = dimension;
		uiRenderer->add(&box);
		
		// Children
		FOREACH(subContext) {
			value->update(dt, uiRenderer);
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
		FOREACH(subContext) {
			value->free();
		}
		subContext.deallocate();
	}

    void CreateUI(Context* context, CreateElementAttribute attr) {
		context->attr = attr.attr;

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
