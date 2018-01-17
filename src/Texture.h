#ifndef TEXTURE_H
#define TEXTURE_H
#include "gl_includes.h"

class Texture {
 public:
  Texture();
  Texture(const char* path);
  GLuint get_id() { return mTextureId; };
 private:
  GLuint mTextureId;
  GLint mWidth, mHeight;
};

#endif
