#ifndef TERRAIN_H
#define TERRAIN_H
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Matrix4x4f.h"
#include "String.h"
#include "ShaderUniformMapping.h"
#include "Constants.h"
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
};

struct TerrainTexture {
    GLuint textures[3] = { 0, 0, 0 }; // Diffuse, specular, normal
};

struct Terrain {
    Matrix4x4f model;
    Mesh mMesh;
    GenerationParameters mParams;
    bool isInitialized = false;
    TerrainTexture textures[Constants::Rendering::MAX_TEXTURES_PER_MESH] = {0, 0, 0}; // Grass, dirt, snow, sand

    void initialize(const GenerationParameters& params);
    void loadTextures(const GenerationParameters& params);
    void render(const ModelUniformMapping& mapping, bool withMaterial = true) const;
    void free();
};

#endif
