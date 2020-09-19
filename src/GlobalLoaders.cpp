#include "GlobalLoaders.h"

TextureLoader GlobalTextureLoader;

void GlobalLoaders::initialize() {
	GlobalTextureLoader.textureList.allocate(16);
}

void GlobalLoaders::free() {
	GlobalTextureLoader.textureList.deallocate();
}