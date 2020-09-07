#include "Terrain.h"
#include "Logger.h"
#include "OpenSimplexNoise.h"
#include "Vertex.h"
#include "TextureLoader.h"

enum class TerrainClassification {
    DIRT,
    DIRT_GRASS,
    GRASS,
    SNOW_GRASS,
    SNOW
};

inline static float randomFloat(float min, float max) {
    return (min + (rand() / (static_cast<float>(RAND_MAX) / (max - min))));
}

void loadTerrainTexture(TerrainTexture& terrainTexture, String& path) {
    String diffusePath = path;
    diffusePath.append("diffuse.jpg");
    terrainTexture.textures[0] = TextureLoader::loadRGBATileTexture(diffusePath);

    String specularPath = path;
    specularPath.append("specular.jpg");
    terrainTexture.textures[1] = TextureLoader::loadRGBATileTexture(specularPath);

    String normalPath = path;
    normalPath.append("normal.jpg");
    terrainTexture.textures[2] = TextureLoader::loadRGBATileTexture(normalPath);

    diffusePath.deallocate();
    specularPath.deallocate();
    normalPath.deallocate();
}

const char* pathToTextures = "./assets/terrain/textures/";
void Terrain::loadTextures(const GenerationParameters& params) {
    // @TODO: Assuming all directory names exist for now
    {
        String grassPath = pathToTextures;
        grassPath.append("grass/");
        loadTerrainTexture(textures[0], grassPath);
        grassPath.deallocate();
    }

    {
        String dirtPath = pathToTextures;
        dirtPath.append("sand/");
        loadTerrainTexture(textures[1], dirtPath);
        dirtPath.deallocate();
    }

    {
        String snowPath = pathToTextures;
        snowPath.append("snow/");
        loadTerrainTexture(textures[2], snowPath);
        snowPath.deallocate();
    }

    {
        // @TODO Investigate using Sampler2DArray or some other thing (maybe one giant
        // terrain texture and a way to index into it
      /*  String sandPath = pathToTextures;
        sandPath.append("sand/");
        loadTerrainTexture(textures[3], sandPath);
        sandPath.deallocate(); */
    }
}

static inline GLfloat calculateTextureCoordinate(int coord, float portionOfTexturePerVertex, int verticesPerTexture, bool dir) {
    if (dir) {
        return (coord % verticesPerTexture) * portionOfTexturePerVertex;
    }

    return ((coord % verticesPerTexture) * portionOfTexturePerVertex);
}

TerrainClassification classifyTerrain(float height, float maxHeight) {
    if (height < 0) {
        height = -height;
        if (height > maxHeight * (2.f / 3.f)) {
            // Upper two-thirds gets dirt
            return TerrainClassification::DIRT;
        } else if (height > maxHeight / 3.f) {
            // Middle two thirds gets mix
            return TerrainClassification::DIRT_GRASS;
        } else {
            return TerrainClassification::GRASS;
        }
    } else {
        if (height > maxHeight * (2.f / 3.f)) {
            // Upper two-thirds gets rock
            return TerrainClassification::SNOW;
        } else if (height > maxHeight / 3.f) {
            // Middle two thirds gets mix
            return TerrainClassification::SNOW_GRASS;
        } else {
            return TerrainClassification::GRASS;
        }
    }
}

void Terrain::initialize(const GenerationParameters& params) {
    if (isInitialized) {
        free();
    }

    Logger::logInfo("Loading textures for terrain...");
    loadTextures(params);

    Logger::logInfo("Generating terrain...");

    float squareSize = ((float)params.size / (float)params.granularity) / 2.f;
    int* perm = getSimplexArray(params.permSize);
    int permIndexCap = (params.permSize / 2) - 1;

    unsigned int numVertices = params.granularity * params.granularity;
    unsigned int numIndices = numVertices * 6;
    GLint* indicies = new GLint[numIndices];
    unsigned int indicesIdx = 0;

    Vertex* vertices = new Vertex[numVertices];
    unsigned int vertexIdx = 0;

    const float portionOfTexturePerVertex = 1.f / (float)params.verticesPerTexture;
    Vector2f vertexCoordinates = { 0.f, 0.f };

    for (int yCoord = 0; yCoord < params.granularity; yCoord++) {
        for (int xCoord = 0; xCoord < params.granularity; xCoord++) {
            float height = calculateSimplexValue(
                Vector2f { static_cast<GLfloat>(xCoord), static_cast<GLfloat>(yCoord) } + squareSize, params.minMaxHeight,
                params.scaleFactor, params.ampFactor, params.frequencyFactor,
                params.numOctaves, perm, permIndexCap);
            Vertex vertex;
            vertex.position = getVec3(squareSize * xCoord, height, squareSize * yCoord);
            vertex.texCoords = vertexCoordinates;

            TerrainClassification classification = classifyTerrain(height, params.minMaxHeight);
            switch (classification) {
            case TerrainClassification::DIRT:
                vertex.textureWeights[0] = 0;
                vertex.textureWeights[1] = 1;
                vertex.textureWeights[2] = 0;
                break;
            case TerrainClassification::DIRT_GRASS:
                vertex.textureWeights[0] = 0.5;
                vertex.textureWeights[1] = 0.5;
                vertex.textureWeights[2] = 0;
                break;
            case TerrainClassification::GRASS:
                vertex.textureWeights[0] = 1;
                vertex.textureWeights[1] = 0;
                vertex.textureWeights[2] = 0;
                break;
            case TerrainClassification::SNOW_GRASS:
                vertex.textureWeights[0] = 0;
                vertex.textureWeights[1] = 0.5;
                vertex.textureWeights[2] = 0.5;
                break;
            case TerrainClassification::SNOW:
                vertex.textureWeights[0] = 0;
                vertex.textureWeights[1] = 0;
                vertex.textureWeights[2] = 1;
                break;

            }

            vertices[vertexIdx++] = vertex;
            if (yCoord != params.granularity - 1 && xCoord != params.granularity - 1) {
                GLuint idx = yCoord * params.granularity + xCoord;
                indicies[indicesIdx++] = idx;
                indicies[indicesIdx++] = idx + 1;
                indicies[indicesIdx++] = idx + params.granularity;
                indicies[indicesIdx++] = idx + params.granularity;
                indicies[indicesIdx++] = idx + params.granularity + 1;
                indicies[indicesIdx++] = idx + 1;
            }

            vertexCoordinates.x += portionOfTexturePerVertex;
        }

        vertexCoordinates.x = 0.f;
        vertexCoordinates.y += portionOfTexturePerVertex;
    }

    // Calculate normals, tangents, and bitangents
    for (size_t indexIndex = 0; indexIndex < numIndices; indexIndex += 3) {
        if (indexIndex + 2 >= numIndices) {
            break;
        }

        GLint thirdIndex = indicies[indexIndex + 2];
        if (static_cast<size_t>(thirdIndex) >= numVertices) {
            break;
		}

        GLint firstIndex = indicies[indexIndex];
        GLint secondIndex = indicies[indexIndex + 1];

        Vertex& firstVertex = vertices[firstIndex];
        Vertex& secondVertex = vertices[secondIndex];
        Vertex& thirdVertex = vertices[thirdIndex];

        Vector3f normal = normalize(cross(vertices[firstIndex].position - vertices[secondIndex].position, 
			vertices[thirdIndex].position - vertices[secondIndex].position));
        firstVertex.normal = normal;
        secondVertex.normal = normal;
        thirdVertex.normal = normal;

        // @TODO: This is copy and pasted in `main.cpp` of the ModelTransformTool.
        // Need to refactor that ASAP
        Vector3f deltaPosition1 = secondVertex.position - firstVertex.position;
        Vector3f deltaPosition2 = thirdVertex.position - firstVertex.position;

        Vector2f deltaUV1 = secondVertex.texCoords - firstVertex.texCoords;
        Vector2f deltaUV2 = thirdVertex.texCoords - firstVertex.texCoords;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        Vector3f tangent = (deltaPosition1 * deltaUV2.y - deltaPosition2 * deltaUV1.y) * r;
        tangent = normalize(tangent - (normal * dotProduct(tangent, normal)));
        Vector3f bitangent = cross(normal, tangent);

        firstVertex.tangent = tangent;
        secondVertex.tangent = tangent;
        thirdVertex.tangent = tangent;

        firstVertex.bitangent = bitangent;
        secondVertex.bitangent = bitangent;
        thirdVertex.bitangent = bitangent;
    }

    Logger::logInfo("Finished generating terrain!");

    // Set up the material
    LoadMaterial material;
    material.diffuse = Vector3f { 0, 0.0, 0 };
    material.ambient = Vector3f { 0, 0.0, 0 };
    material.specular = Vector3f { 0, 0.0, 0 };
    mMesh.initialize(vertices, numVertices, indicies, numIndices, material);

    for (int textureIndex = 0; textureIndex < Constants::Rendering::MAX_TEXTURES_PER_MESH; textureIndex++) {
        mMesh.material.textureList.add(TextureType::DIFFUSE, textures[textureIndex].textures[0]);
        mMesh.material.textureList.add(TextureType::SPECULAR, textures[textureIndex].textures[1]);
        mMesh.material.textureList.add(TextureType::NORMAL, textures[textureIndex].textures[2]);
    }

    int halfMapSize = squareSize * (params.granularity / 2);
	model = Matrix4x4f { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
    model = translateMatrix(model, getVec3(-halfMapSize, 0, -halfMapSize));
    delete []vertices;
    delete []indicies;

    mParams = params;
    isInitialized = true;
}

void Terrain::render(const ModelUniformMapping& mapping, bool withMaterial) const {
    if (!isInitialized) {
        return;
    }

	setShaderMat4(mapping.MODEL, model);
	setShaderBool(mapping.DISABLE_BONES, true);
	mMesh.render(mapping.materialUniformMapping, withMaterial);

}

void Terrain::free() {
    mMesh.free();
    isInitialized = false;
    glDeleteTextures(9, textures->textures);
}
