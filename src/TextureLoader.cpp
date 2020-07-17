#include "TextureLoader.h"
#include "Logger.h"
#include <SOIL.h>

GLuint TextureLoader::loadRGBATileTexture(String path) {
	GLuint texture;
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	const char* cStrPath = path.value;
    unsigned char* image = SOIL_load_image(cStrPath, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == NULL) {
		Logger::logError("Unable to load image from path: ");
		return 0;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture;
}