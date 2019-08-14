#ifndef TERRAIN_H
#define TERRAIN_H
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
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
    void render(const Shader& shader, bool withMaterial = true) const;
    void free();
private:
	bool mHasGenerated = false;
    Mesh mMesh;
    glm::mat4 mModel;
};

void to_json(json& j, const Terrain& terrain);
void from_json(const json& j, Terrain& terrain);

#endif
