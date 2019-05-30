#pragma once
#include <GL/glew.h>

class Shader;
class DeferredGeometryBuffer {
public:
	void generate();
    void free_resources();
    void render(const Shader& shader);
private:
    GLuint mBuffer;
    GLuint mPositionTexture;
    GLuint mNormalTexture;
    GLuint mColorTexture;
    bool mHasGenerated = false;
    GLuint* mAttachments;
};