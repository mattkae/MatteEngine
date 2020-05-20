#pragma once

// @TODO Flesh out this idea a little bit more
enum UIEventType {
	NONE = 0,
	SHOW_MODEL = 1,
	EDIT_TRANSLATION_X,
	EDIT_TRANSLATION_Y,
	EDIT_TRANSLATION_Z,
	EDIT_SCALE_X,
	EDIT_SCALE_Y,
	EDIT_SCALE_Z,
};

struct UIEvent {
	UIEventType type = UIEventType::NONE;
	void* data = nullptr;
};

struct UIEventProcessor {
	UIEvent activeEvent;
	void processEvent();
};