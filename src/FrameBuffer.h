#pragma once

#include <GL/glew.h>

struct TextureFrameBuffer {
	GLuint fbo;
	GLuint texture;
	GLuint depthBuffer;
};

namespace FrameBuffer {
	TextureFrameBuffer createFrameBufferRGBA(int width, int height);
	void freeFrameBuffer(TextureFrameBuffer* buffer);
};

TextureFrameBuffer createFloatingPointFrameBuffer(int width, int height);