#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <iostream>
#include <map>
#include <string>

using namespace std;


enum MaterialType {
  TEX_DIFFUSE = 0,
  TEX_SPECULAR
};

struct Texture {
  GLuint id;
  GLint width = 0;
  GLint height = 0;
  MaterialType type = MaterialType::TEX_DIFFUSE;
};

Texture create_blank();
Texture load_texture(const char* path);

static std::map<std::string, Texture> loadedTextures;

#endif
