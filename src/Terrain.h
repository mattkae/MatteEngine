#ifndef TERRAIN_H
#define TERRAIN_H
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Matrix4x4f.h"
#include <GL/glew.h>

struct GenerationParameters {
    int size = 512;
    int granularity = 64;
    int permSize = 64;
    float scaleFactor = 0.02f;
    float minMaxHeight = 10.f;
    float ampFactor = 0.5f;
    float frequencyFactor = 1.8f;
    int numOctaves = 16;
};

struct Terrain {
    Matrix4x4f model;
    Mesh mMesh;

    void initialize(const GenerationParameters& params);
    void render(const Shader& shader, bool withMaterial = true) const;
    void free();
};

#endif
