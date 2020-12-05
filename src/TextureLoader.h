#pragma once
#include "GeneratedTexture.h"
#include "List.h"
#include "MyString.h"

struct TextureLoader {
	List<GLuint> textureList;
	GLuint loadRGBATileTexture(String& path);
	GLuint createTexture(int width, int height);
	bool deleteTexture(GLuint texture);
	GLuint loadRGBATexture(String& path);
	void registerDepthTexture(GLuint depthTexture) {
		textureList.add(depthTexture);
	}
};
