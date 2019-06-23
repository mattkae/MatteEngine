#pragma once
#include <GL/glew.h>
#include <vector>
#include "Shader.h"
#include "Quad.h"

class Model;
class Camera;
class Shader;
class DeferredGeometryBuffer {
public:
	void generate();
    void free();
    void render(const Camera& camera, const std::vector<Model> models) const;
    void bindTextures(const Shader& shader) const;
    void applyDepth() const;
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