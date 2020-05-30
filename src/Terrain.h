#ifndef TERRAIN_H
#define TERRAIN_H
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Matrix4x4f.h"
#include "String.h"
#include <GL/glew.h>

struct GenerationParameters {
    int size = 1000;
    int granularity = 100;
    int permSize = 1000;
    float scaleFactor = 0.004f;
    float minMaxHeight = 64.f;
    float ampFactor = 0.6f;
    float frequencyFactor = 2.f;
    int numOctaves = 64;
    int verticesPerTexture = 16;

    String grassTexturePath;
    String rockTexturePath;
    String dirtTexturePath;
};

struct Terrain {
    Matrix4x4f model;
    Mesh mMesh;
    GenerationParameters mParams;
    bool isInitialized = false;

    GLuint textures[3] = { 0, 0, 0 }; // Grass, dirt, rock

    void initialize(const GenerationParameters& params);
    void loadTextures(const GenerationParameters& params);
    void render(const Shader& shader, bool withMaterial = true) const;
    void free();
};

#endif
