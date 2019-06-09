#include "DeferredGeometryBuffer.h"
#include "Constants.h"
#include "Model.h"

void DeferredGeometryBuffer::generate() {
    mShader.load("src/shaders/GBufferShader.vert");

    glGenBuffers(1, &this->mBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->mBuffer);

	glGenTextures(1, &this->mPositionTexture);
    glBindTexture(GL_TEXTURE_2D, this->mPositionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Constants.width, Constants.height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mPositionTexture, 0);

	glGenTextures(1, &this->mNormalTexture);
    glBindTexture(GL_TEXTURE_2D, this->mNormalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Constants.width, Constants.height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->mNormalTexture, 0);

	glGenTextures(1, &this->mColorTexture);
	glBindTexture(GL_TEXTURE_2D, this->mColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Constants.width, Constants.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->mNormalTexture, 0);

	mAttachments = new GLuint[3]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, mAttachments);
    mHasGenerated = true;
}

void DeferredGeometryBuffer::free() {
    if (mHasGenerated) {
        glDeleteFramebuffers(1, &this->mBuffer);
        glDeleteTextures(1, &this->mPositionTexture);
        glDeleteTextures(1, &this->mNormalTexture);
        glDeleteTextures(1, &this->mColorTexture);
        delete mAttachments;
        mHasGenerated = false;
	}
}

void DeferredGeometryBuffer::render(const std::vector<Model> models) const {
    mShader.use();
    glBindFramebuffer(GL_FRAMEBUFFER, this->mBuffer);

    for (auto model: models) {
        model.render(mShader, true);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredGeometryBuffer::bindTextures() const {
    
}