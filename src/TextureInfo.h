#pragma once
#include "MyString.h"
#include <GL/glew.h>

class BinarySerializer;

struct TextureInfo {
    String fullpath;
    GLuint uniqueId = 0;

    void write(BinarySerializer& serializer);
    void read(BinarySerializer& serializer);
};

