#pragma once

enum class UIEventType {
	NONE = 0,
	SHOW_MODEL = 1,
	SHOW_TERRAIN = 2,
	EDIT_TRANSLATION_X = 3,
	EDIT_TRANSLATION_Y = 4,
	EDIT_TRANSLATION_Z = 5,
	EDIT_SCALE_X = 6,
	EDIT_SCALE_Y = 7,
	EDIT_SCALE_Z = 8,
	EDIT_ROTATION_X = 9,
	EDIT_ROTATION_Y = 10,
	EDIT_ROTATION_Z = 11,
	HIDE_MODEL = 12
};

struct UIEvent {
	UIEventType type = UIEventType::NONE;
	void* data = nullptr;
};

struct Scene;

struct UIEventProcessor {
	Scene* scene = nullptr;
	UIEvent activeEvent;
	void addEvent(UIEvent& event);
	void processEvent();
	void hideModelPanel();

	int rightPanelIdx = -1;
};