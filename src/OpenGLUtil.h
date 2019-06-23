#include <GL/glew.h>

namespace OpenGLUtil {
    GLint* getWindowDimensions() {
        GLint* dims = new GLint[4];
        glGetIntegerv(GL_VIEWPORT, dims);
        GLint fbWidth = dims[2];
        GLint fbHeight = dims[3];
        return dims;
    }
}