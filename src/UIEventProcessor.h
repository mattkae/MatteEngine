#pragma once
#include "UIEventType.h"

struct UIEvent {
	UIEventType type = UIEventType::NONE;
	void* data = nullptr;
};

struct Scene;
struct EditorUI;

struct UIEventProcessor {
	void processEvent(UIEvent& activeEvent);
};