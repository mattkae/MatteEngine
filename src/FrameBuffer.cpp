#include "FrameBuffer.h"
#include "GlobalLoaders.h"

TextureFrameBuffer FrameBuffer::createFrameBufferRGBA(int width, int height) {
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint texColorBuffer = GlobalTextureLoader.createTexture(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return { framebuffer, texColorBuffer };
}