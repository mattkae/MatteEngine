#pragma once
#include "GeneratedTexture.h"
#include "List.h"
#include "MyString.h"

// @TODO Figure out how to do this in a general way
struct TextureLoader {
	List<GeneratedTexture> textureList;
	static GLuint loadRGBATileTexture(String path);
};