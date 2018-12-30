#ifndef TERRAIN_H
#define TERRAIN_H
#include "Camera.h"
#include "Shader.h"
#include <GL/glew.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
    float minMaxHeight = DEFAULT_HEIGHT;
    float ampFactor = DEFAULT_AMP_FACTOR;
    float frequencyFactor = DEFAULT_FREQUENCY_FACTOR;
    int numOctaves = DEFAULT_NUM_OCTAVES;
};

struct Terrain {
 public:
    bool generate(const GenerationParameters& params);
    void render(const Shader& shader, const Camera &camera) const;
    void free();
    bool isInited() const { return hasGenerated; }
    bool wireframeMode = false;
 private:
    int numIndices = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    bool hasGenerated = false;
};

void to_json(json &j, const Terrain& terrain);
void from_json(const json &j, Terrain& terrain);

#endif
