#pragma once

#include <GL/glew.h>

struct TextureFrameBuffer {
	GLuint fbo;
	GLuint texture;
};

namespace FrameBuffer {
	TextureFrameBuffer createFrameBufferRGBA(int width, int height);
};