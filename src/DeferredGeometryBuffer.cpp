#include "DeferredGeometryBuffer.h"
#include "GlobalApplicationState.h"
#include "Logger.h"
#include "Scene.h"
#include "OpenGLUtil.h"

GLuint generateColorTexture(GLint width, GLint height) {
	GLuint texture = 0;
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return texture;
}

void DeferredGeometryBuffer::generate()
{
    if (mHasGenerated) {
        free();
    }

    GLint* dims = OpenGLUtil::getWindowDimensions();
    this->width = dims[2];
    this->height = dims[3];
    delete dims;

    GLenum err;
    mShader = loadShader("src/shaders/GBufferShader.vert", "src/shaders/GBufferShader.frag");

    glGenFramebuffers(1, &this->mBuffer);

    glGenTextures(1, &this->mPositionTexture);
    glBindTexture(GL_TEXTURE_2D, this->mPositionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &this->mNormalTexture);
    glBindTexture(GL_TEXTURE_2D, this->mNormalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width, this->height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	mDiffuseTexture = generateColorTexture(width, height);
	mSpecularTexture = generateColorTexture(width, height);
	mEmissiveTexture = generateColorTexture(width, height);
	mMaterialInfoTexture = generateColorTexture(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, this->mBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mPositionTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->mNormalTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->mDiffuseTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->mSpecularTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, this->mEmissiveTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, this->mMaterialInfoTexture, 0);
    mAttachments = new GLuint[6] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, 
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
    glDrawBuffers(6, mAttachments);

    glGenRenderbuffers(1, &this->mDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, this->mDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width, this->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->mDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Logger::logError("GBuffer framebuffer is not okay.");
    }

    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error during generate: %d\n", err);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mQuad.generate();
    mHasGenerated = true;
}

void DeferredGeometryBuffer::free()
{
    if (mHasGenerated) {
        glDeleteFramebuffers(1, &this->mBuffer);
        glDeleteTextures(1, &this->mPositionTexture);
        glDeleteTextures(1, &this->mNormalTexture);
        glDeleteTextures(1, &this->mDiffuseTexture);
        glDeleteTextures(1, &this->mSpecularTexture);
        glDeleteTextures(1, &this->mEmissiveTexture);
        glDeleteTextures(1, &this->mMaterialInfoTexture);
        glDeleteBuffers(1, &this->mDepth);
        delete mAttachments;
        glDeleteProgram(this->mShader);
        mHasGenerated = false;
    }
}

void DeferredGeometryBuffer::renderToBuffer(const BetterCamera& camera, const BetterScene& scene) const
{
    if (!mHasGenerated) {
        return;
    }

    glClearColor(0, 0, 0, 0.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, this->mBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	useShader(mShader);
    renderCamera(camera, mShader);
	renderModels(scene, mShader, true);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredGeometryBuffer::renderToScreen(const Shader& shader) const
{
    if (!mHasGenerated) {
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mPositionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mNormalTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mDiffuseTexture);
	glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mSpecularTexture);
	glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mEmissiveTexture);
	glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mMaterialInfoTexture);

    mQuad.render();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->mBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
    glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}