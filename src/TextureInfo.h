#pragma once
#include <string>
#include <GL/glew.h>

class BinarySerializer;

struct TextureInfo {
    std::string fullpath;
    GLuint uniqueId = 0;

    void write(BinarySerializer& serializer);
    void read(BinarySerializer& serializer);
};

