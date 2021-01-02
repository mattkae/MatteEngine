#ifndef TERRAIN_H
#define TERRAIN_H
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Matrix4x4f.h"
#include "MyString.h"
#include "ShaderUniformMapping.h"
#include "Constants.h"
#include <GL/glew.h>

struct Camera;
struct LightSystem;

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
	GLuint diffuse = 0;
	GLuint specular = 0;
	GLuint normal = 0;
};

struct Terrain {
	GLuint mVao = 0;
	GLuint mVbo = 0;
	GLuint mEbo = 0;
	int numIndices = 0;
    Matrix4x4f model;
    GenerationParameters mParams; // @TODO: This doesn't really have to be here
    bool isInitialized = false;
    TerrainTexture textureList[3] = {0, 0, 0}; // Grass, dirt, snow

    void initialize(const GenerationParameters& params);
    void loadTextures(const GenerationParameters& params);
    void render(const Camera* camera, const LightSystem* lightSystem) const;
    void free();
};

#endif
