#ifndef IMAGE_DRAWER_H
#define IMAGE_DRAWER_H

#include "gl_includes.h"
#include "Shader.h"

class Texture;

class ImageDrawer {
 public:
  ImageDrawer();
  ImageDrawer(Texture* texture);
  void render();
  void render(GLuint id);
 private:
  void gen_buffers();
  
  GLuint mVbo, mVao, mEbo;
  Texture* mTexture;
  Shader mShader;
};

#endif
