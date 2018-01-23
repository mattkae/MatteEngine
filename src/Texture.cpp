#include "Texture.h"

Texture create_blank() {
  Texture texture;
  glGenTextures(1, &texture.id);
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);

  texture.width = 0;
  texture.height = 0;

  return texture;
}

Texture load_texture(const char* path) {
  // TO-DO: Check is loaded here!
  GLuint id;
  
  glGenTextures(1, &id);
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  GLint w, h;
  unsigned char* image = SOIL_load_image(path, &w, &h, 0, SOIL_LOAD_RGBA);
  if (!image) {
    cerr << "Unable to load image from path " << path << endl;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);

  Texture texture;
  texture.width = w;
  texture.height = h;
  texture.id = id;

  return texture;
}
