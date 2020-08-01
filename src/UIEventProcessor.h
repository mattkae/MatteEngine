#pragma once

enum class UIEventType {
	NONE = 0,

	CLOSE_PANEL,

	SHOW_MODEL,
	EDIT_TRANSLATION_X,
	EDIT_TRANSLATION_Y,
	EDIT_TRANSLATION_Z,
	EDIT_SCALE_X,
	EDIT_SCALE_Y,
	EDIT_SCALE_Z,
	EDIT_ROTATION_X,
	EDIT_ROTATION_Y,
	EDIT_ROTATION_Z,
	HIDE_MODEL,

	SHOW_TERRAIN,
	EDIT_TERRAIN_SIZE,
	EDIT_TERRAIN_GRANULARITY,
	EDIT_TERRAIN_PERM,
	EDIT_TERRAIN_SCALE,
	EDIT_TERRAIN_MIN_MAX,
	EDIT_TERRAIN_AMP,
	EDIT_TERRAIN_FREQ,
	EDIT_TERRAIN_OCT,
	EDIT_TERRAIN_NUM_VERTICES,
	EDIT_TERRAIN_APPLY,
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