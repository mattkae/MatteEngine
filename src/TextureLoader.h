#pragma once
#include "TextureListItem.h"
#include "List.h"
#include "String.h"

// @TODO Figure out how to do this in a general way
struct TextureLoader {
	List<TextureListItem> textureList;
	static GLuint loadRGBATexture(String path);
};