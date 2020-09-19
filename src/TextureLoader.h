#pragma once
#include "GeneratedTexture.h"
#include "List.h"
#include "MyString.h"

// @TODO Figure out how to do this in a general way
struct TextureLoader {
	List<GLuint> textureList;
	GLuint loadRGBATileTexture(String path);
	GLuint createTexture(int width, int height);
};
