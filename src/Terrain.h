#ifndef TERRAIN_H
#define TERRAIN_H
#include "gl_includes.h"
#include "Shader.h"
#include "Camera.h"

const int DEFAULT_SIZE = 512;
const int DEFAULT_GRANULARITY = 64;
const int DEFAULT_PERM = 512;
const float DEFAULT_SCALE = 0.02f;
const float DEFAULT_HEIGHT = 10.f;
const float DEFAULT_AMP_FACTOR = 0.5f;
const float DEFAULT_FREQUENCY_FACTOR = 1.8f;
const int DEFAULT_NUM_OCTAVES = 16;

struct GenerationParameters {
  int size = DEFAULT_SIZE;
  int granularity = DEFAULT_GRANULARITY;
  int permSize = DEFAULT_PERM;
  float scaleFactor = DEFAULT_SCALE;
  float  minMaxHeight = DEFAULT_HEIGHT;
  float ampFactor = DEFAULT_AMP_FACTOR;
  float frequencyFactor = DEFAULT_FREQUENCY_FACTOR;
  int numOctaves = DEFAULT_NUM_OCTAVES;
};

struct Terrain {
  int numIndices = 0;
  GLuint vao = 0;
  GLuint vbo = 0;
  GLuint ebo = 0;
  bool wireframeMode = false;
};

Terrain generate_terrain(int size, int granularity, int permSize = 512, float maxHeight = DEFAULT_HEIGHT,float scaleFactor = DEFAULT_SCALE, float ampFactor = DEFAULT_AMP_FACTOR, float frequencyFactor = DEFAULT_FREQUENCY_FACTOR, int numOctaves = DEFAULT_NUM_OCTAVES);
Terrain generate_terrain(GenerationParameters params);
void render_terrain(const Terrain& terrain, const Shader& shader, const Camera& camera);

#endif