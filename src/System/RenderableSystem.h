#pragma once

#include "../Types.h"
#include "../List.h"
#include "../Model.h"
#include "../ShaderUniformMapping.h"
#include "../Box.h"

struct RenderableEntity {
	u8 mEntityId = 0;
	bool mShouldRender = true;
	Model mModel;
};

struct RenderableSystem {
	List<RenderableEntity> mEntities;
	void initialize();
	void update(float dtMs);
	void render(const ModelUniformMapping& mapping, bool withMaterial = true) const;
	void free();
};