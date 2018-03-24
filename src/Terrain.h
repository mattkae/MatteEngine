#ifndef TERRAIN_H
#define TERRAIN_H
#include "gl_includes.h"
#include "Shader.h"
#include "Camera.h"

struct Terrain {
  int numIndices = 0;

  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  bool wireframeMode = false;
};

Terrain generate_terrain(int dimension, int granularity);
void render_terrain(const Terrain& terrain, const Shader& shader, const Camera& camera);

#endif
