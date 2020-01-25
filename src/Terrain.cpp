#include "Terrain.h"
#include "Logger.h"
#include "OpenSimplexNoise.h"

static float random_float(float min, float max)
{
    return (min + (rand() / (static_cast<float>(RAND_MAX) / (max - min))));
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
				Vector2f { static_cast<GLfloat>(xCoord), static_cast<GLfloat>(yCoord) } + squareSize, params.minMaxHeight,
                params.scaleFactor, params.ampFactor, params.frequencyFactor,
                params.numOctaves, perm, permIndexCap);
            Vector3f position = getVec3(squareSize * xCoord, height, squareSize * yCoord);

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

        auto normal = normalize(cross(vertices[firstIndex].position - vertices[secondIndex].position, 
			vertices[thirdIndex].position - vertices[secondIndex].position));
        vertices[firstIndex].normal = normal;
        vertices[secondIndex].normal = normal;
        vertices[thirdIndex].normal = normal;
    }

    Logger::logInfo("Finished generating terrain!");

    terrain.mMesh.indicies = indices;
    terrain.mMesh.vertices = vertices;
	terrain.mMesh.material.diffuse = { 0, 0.2, 0 };
	initializeMesh(terrain.mMesh);

    int halfMapSize = params.granularity / 2;
	terrain.model = Matrix4x4f { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
    terrain.model = translateMatrix(terrain.model, getVec3(-halfMapSize, -50, -halfMapSize));
}

void renderTerrain(const Terrain& terrain, const Shader& shader, bool withMaterial) {
	setShaderMat4(shader, "uModel", terrain.model);
	renderMesh(terrain.mMesh, shader, withMaterial);
}

void freeTerrain(Terrain& terrain) {
	freeMesh(terrain.mMesh);
}
