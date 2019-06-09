#pragma once
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

class Model;
class DeferredGeometryBuffer {
public:
	void generate();
    void free();
    void render(const std::vector<Model> models) const;
    void bindTextures() const;
private:
    GLuint mBuffer;
    GLuint mPositionTexture;
    GLuint mNormalTexture;
    GLuint mColorTexture;
    bool mHasGenerated = false;
    GLuint* mAttachments;
    Shader mShader;
};