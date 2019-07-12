#pragma once
#include <GL/glew.h>
#include <vector>
#include "Shader.h"
#include "Quad.h"

class Scene;
class Camera;
class Shader;
class DeferredGeometryBuffer {
public:
	void generate();
    void free();
    void renderToBuffer(const Camera& camera, const Scene* scene) const;
    void renderToScreen(const Shader& shader) const;
private:
    GLuint mBuffer;
    GLuint mPositionTexture;
    GLuint mNormalTexture;
    GLuint mColorTexture;
    GLuint mDepth;
    bool mHasGenerated = false;
    GLuint* mAttachments;
    Shader mShader;
    Quad mQuad;

    GLint width;
    GLint height;
};