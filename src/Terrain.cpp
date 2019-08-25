#include "Terrain.h"
#include "Logger.h"
#include "OpenSimplexNoise.h"
#include "GlmUtility.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

static float random_float(float min, float max)
{
    return (min + (rand() / (static_cast<float>(RAND_MAX) / (max - min))));
}

glm::vec3 calculateColor(float height, float max) {
    glm::vec3 color;
    if (height > max * 0.98f) {
        color = glm::vec3(random_float(0.95f, 1.0f));
    } else if (height > max * 0.85f) {
        color = glm::vec3(0.84, 0.74, 0.84);
    } else if (height > (-max) * 0.25f) {
        color = glm::vec3(0.0, random_float(0.5f, 0.6f), 0.1);
    } else if (height > (-max) * 0.35f) {
        color = glm::vec3(0.925, 0.78, 0.68);
    } else {
        color = glm::vec3(0.1, random_float(0.3f, 0.5f), random_float(0.8f, 1.f));
    }

    return color;
}

void to_json(json& j, const Terrain& terrain) {
    // TODO!
}

void from_json(const json& j, Terrain& terrain) {
    GenerationParameters genParams;
    j.at("size").get_to<int>(genParams.size);
    j.at("granularity").get_to<int>(genParams.granularity);
    j.at("permSize").get_to<int>(genParams.permSize);
    j.at("scaleFactor").get_to<float>(genParams.scaleFactor);
    j.at("minMaxHeight").get_to<float>(genParams.minMaxHeight);
    j.at("ampFactor").get_to<float>(genParams.ampFactor);
    j.at("frequencyFactor").get_to<float>(genParams.frequencyFactor);
    j.at("numOctaves").get_to<int>(genParams.numOctaves);
    initializeTerrain(terrain, genParams);
}

void initializeTerrain(Terrain& terrain, const GenerationParameters& params) {
    Logger::logInfo("Generating terrain...");

    float squareSize = ((float)params.size / (float)params.granularity) / 2.f;
    int* perm = getSimplexArray(params.permSize);
    int permIndexCap = (params.permSize / 2) - 1;
    std::vector<Vertex> vertices;
    std::vector<GLint> indices;

    for (int yCoord = 0; yCoord < params.granularity; yCoord++) {
        for (int xCoord = 0; xCoord < params.granularity; xCoord++) {
            float height = calculateSimplexValue(
                squareSize + glm::vec2(xCoord, yCoord), params.minMaxHeight,
                params.scaleFactor, params.ampFactor, params.frequencyFactor,
                params.numOctaves, perm, permIndexCap);
            glm::vec3 position = glm::vec3(squareSize * xCoord, height, squareSize * yCoord);

            vertices.push_back({ position });

            if (yCoord != params.granularity - 1 && xCoord != params.granularity - 1) {
                GLuint idx = yCoord * params.granularity + xCoord;
                indices.push_back(idx);
                indices.push_back(idx + 1);
                indices.push_back(idx + params.granularity);
                indices.push_back(idx + params.granularity);
                indices.push_back(idx + params.granularity + 1);
                indices.push_back(idx + 1);
            }
        }
    }

    // Calculate normals
    for (size_t indexIndex = 0; indexIndex < indices.size(); indexIndex += 3) {
        if (indexIndex + 2 >= indices.size()) {
            break;
        }

        auto thirdIndex = indices.at(indexIndex + 2);
        if (static_cast<size_t>(thirdIndex) >= vertices.size()) {
            break;
		}

        auto firstIndex = indices.at(indexIndex);
        auto secondIndex = indices.at(indexIndex + 1);

        auto normal = glm::normalize(glm::cross(vertices[firstIndex].position - vertices[secondIndex].position, 
			vertices[thirdIndex].position - vertices[secondIndex].position));
        vertices[firstIndex].normal = normal;
        vertices[secondIndex].normal = normal;
        vertices[thirdIndex].normal = normal;
    }

    Logger::logInfo("Finished generating terrain!");

    terrain.mMesh.indicies = indices;
    terrain.mMesh.vertices = vertices;
    terrain.mMesh.material.diffuse = glm::vec3(0, 0.2, 0);
	initializeMesh(terrain.mMesh);

    int halfMapSize = params.granularity / 2;
    terrain.model = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    terrain.model = glm::translate(terrain.model, glm::vec3(-halfMapSize, -50, -halfMapSize));
}

void renderTerrain(const Terrain& terrain, const Shader& shader, bool withMaterial) {
	setShaderMat4(shader, "uModel", terrain.model);
	renderMesh(terrain.mMesh, shader, withMaterial);
}

void freeTerrain(Terrain& terrain) {
	freeMesh(terrain.mMesh);
}
