#ifndef TERRAIN_H
#define TERRAIN_H
#include "Camera.h"
#include "Shader.h"
#include <GL/glew.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
public:
    bool generate(const GenerationParameters& params);
    void render(const Shader& shader, const Camera& camera) const;
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

void to_json(json& j, const Terrain& terrain);
void from_json(const json& j, Terrain& terrain);

#endif
