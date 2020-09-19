#pragma once
#include "TextureLoader.h"

extern TextureLoader GlobalTextureLoader;

namespace GlobalLoaders {
	void initialize();
	void free();
};