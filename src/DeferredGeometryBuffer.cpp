#include "DeferredGeometryBuffer.h"
#include "Camera.h"
#include "Constants.h"
#include "Logger.h"
#include "Scene.h"
#include "OpenGLUtil.h"
#include "Shader.h"

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
    mShader.load("src/shaders/GBufferShader.vert", "src/shaders/GBufferShader.frag");

    glGenFramebuffers(1, &this->mBuffer);

    glGenTextures(1, &this->mPositionTexture);
    glBindTexture(GL_TEXTURE_2D, this->mPositionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width, this->height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &this->mNormalTexture);
    glBindTexture(GL_TEXTURE_2D, this->mNormalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width, this->height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &this->mColorTexture);
    glBindTexture(GL_TEXTURE_2D, this->mColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, this->mBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mPositionTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->mNormalTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->mColorTexture, 0);
    mAttachments = new GLuint[3] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, mAttachments);

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
        glDeleteTextures(1, &this->mColorTexture);
        glDeleteBuffers(1, &this->mDepth);
        delete mAttachments;
        mHasGenerated = false;
    }
}

void DeferredGeometryBuffer::renderToBuffer(const Camera& camera, const Scene* scene) const
{
    if (!mHasGenerated) {
        return;
    }

    glClearColor(0, 0, 0, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, this->mBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader.use();
    camera.render(mShader, false);
    scene->renderModels(mShader, true);

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
    glBindTexture(GL_TEXTURE_2D, mColorTexture);

    mQuad.render();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->mBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
    glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}