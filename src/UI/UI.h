#pragma once
#include "../Vector2f.h"
#include "../Vector4f.h"
#include "../List.h"
#include "UIRenderer.h"

namespace UI {

	struct Rectangle;
	struct Button;
	struct Context;

	enum Alignment {
		Alignment_LEFT,
		Alignment_Right,
		Alignment_CENTER
	};

	enum Positioning {
		Positioning_ABSOLUTE,
		Positioning_MIN,
		Positioning_MAX,
		Positioning_CENTER,
		Positioning_FLOW
	};

	enum FlowType {
		FlowType_VERTICAL,
		FlowType_HORIZONTAL
	};

	enum Sizing {
		Sizing_STATIC,
		Sizing_DYNAMIC
	};

	enum ElementType {
		ElementType_NONE,
		ElementType_CONTAINER,
		ElementType_INPUT,
		ElementType_BUTTON
	};

	enum TextInputType {
		TextInputType_NONE,
		TextInputType_INT,
		TextInputType_FLOAT,
		TextInputType_STRING
	};

	struct DimensionAttribute {
		Positioning positioning = Positioning_ABSOLUTE;
		Sizing sizing = Sizing_STATIC;
		float position = 0;
		float fixedSize = 0;
		float percentageSize = 0;
	};

	struct ElementAttribute {
		float padding;
		ElementType type;
	    DimensionAttribute dimensionAttrList[2];
		Alignment alignment = Alignment_LEFT;
		Vector4f backgroundColor;
		Vector4f textColor;
		FlowType flowType;
		TextInputType textInputType;

		void (*onChange)(Context* context) = NULL;
		void (*onMousedown)(Context* context) = NULL;
		void (*onMouseup)(Context* context) = NULL;
	};

	struct CreateElementAttribute {
		ElementAttribute attr;
		List<CreateElementAttribute> subElements;
	};

	struct Context {
		ElementAttribute attr;
		Context* parent = NULL;
		Context* prevNeighbor = NULL;
		Context* nextNeighbor = NULL;
		List<Context> subContext;

		Vector2f position;
		Vector2f dimension;

		BatchedBox box;
		bool show = true;

		void update(float dt, UIRenderer* uiRenderer);
		void render() const;
		void free();
	};

	struct PrimaryContext {
		UIRenderer uiRenderer;
		TextRenderer textRenderer;
		List<Context> contextList;

		void initialize();
		void update(float dt);
		void render() const;
		void free();
	};

	void CreateUI(Context* context, CreateElementAttribute attr);
	
}
