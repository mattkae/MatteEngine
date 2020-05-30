#ifndef TEXTURE_LIST_ITEM_H
#define TEXTURE_LIST_ITEM_H
#include <GL/glew.h>
#include "MeshTexture.h"

struct GeneratedTexture {
	int uniqueId;
	GLuint texture;
	TextureType type;
};

#endif