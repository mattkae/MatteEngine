#include "Terrain.h"
#include "Logger.h"
#include "OpenSimplexNoise.h"
#include "Vertex.h"
#include "TextureLoader.h"

inline static float randomFloat(float min, float max) {
    return (min + (rand() / (static_cast<float>(RAND_MAX) / (max - min))));
}

const char* pathToTextures = "./assets/terrain/textures/";
void Terrain::loadTextures(const GenerationParameters& params) {
    if (params.grassTexturePath.value != nullptr) {
        String pathToGrass;
        pathToGrass = pathToTextures;
        pathToGrass.append(params.grassTexturePath);
        textures[0] = TextureLoader::loadRGBATileTexture(pathToGrass);
    }
    if (params.rockTexturePath.value != nullptr) {
        String pathToRock;
        pathToRock = pathToTextures;
        pathToRock.append(params.rockTexturePath);
        textures[1] = TextureLoader::loadRGBATileTexture(pathToRock);
    }
    if (params.dirtTexturePath.value != nullptr) {
        String pathToDirt;
        pathToDirt = pathToTextures;
        pathToDirt.append(params.dirtTexturePath);
        textures[2] = TextureLoader::loadRGBATileTexture(pathToDirt);
    }
}

static inline GLfloat calculateTextureCoordinate(int coord, float portionOfTexturePerVertex, int verticesPerTexture, bool dir) {
    if (dir) {
        return (coord % verticesPerTexture) * portionOfTexturePerVertex;
    }

    return ((coord % verticesPerTexture) * portionOfTexturePerVertex);
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
            vertex.texCoords = getVec3(vertexCoordinates.x, vertexCoordinates.y, 0.f);
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

    // Calculate normals
    for (size_t indexIndex = 0; indexIndex < numIndices; indexIndex += 3) {
        if (indexIndex + 2 >= numIndices) {
            break;
        }

        auto thirdIndex = indicies[indexIndex + 2];
        if (static_cast<size_t>(thirdIndex) >= numVertices) {
            break;
		}

        GLint firstIndex = indicies[indexIndex];
        GLint secondIndex = indicies[indexIndex + 1];

        Vector3f normal = normalize(cross(vertices[firstIndex].position - vertices[secondIndex].position, 
			vertices[thirdIndex].position - vertices[secondIndex].position));
        vertices[firstIndex].normal = normal;
        vertices[secondIndex].normal = normal;
        vertices[thirdIndex].normal = normal;
    }

    Logger::logInfo("Finished generating terrain!");

    LoadMaterial material;
    material.diffuse = Vector3f { 0, 0.2, 0 };
    material.ambient = Vector3f { 0, 0.2, 0 };
    material.specular = Vector3f { 0, 0.2, 0 };
    mMesh.initialize(vertices, numVertices, indicies, numIndices, material);

    if (textures[0] > 0) {
        mMesh.material.diffuseTexture = textures[0];
    }

    int halfMapSize = squareSize * (params.granularity / 2);
	model = Matrix4x4f { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
    model = translateMatrix(model, getVec3(-halfMapSize, -50, -halfMapSize));
    delete []vertices;
    delete []indicies;

    mParams = params;
    isInitialized = true;
}

void Terrain::render(const Shader& shader, bool withMaterial) const {
    if (!isInitialized) {
        return;
    }

	setShaderMat4(shader, "uModel", model);
	setShaderBool(shader, "uDisableBones", true);
	mMesh.render(shader, withMaterial);

}

void Terrain::free() {
    mMesh.free();
    isInitialized = false;
    glDeleteTextures(3, textures);
}
