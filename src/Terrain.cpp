#include "Terrain.h"
#include "Logger.h"
#include "OpenSimplexNoise.h"
#include "Vertex.h"
#include "GlobalLoaders.h"
#include "System/LightSystem.h"
#include "List.h"

enum class TerrainClassification {
	GRASS = 0,
    DIRT = 1,
    SNOW = 2
};

struct TerrainVertex {
	Vector3f position;
	Vector3f normal;
	Vector2f texCoords;
	Vector3f tangent;
	Vector3f bitangent;
	float layer;
};

inline static float randomFloat(float min, float max) {
    return (min + (rand() / (static_cast<float>(RAND_MAX) / (max - min))));
}

void loadTerrainTexture(TerrainTexture& terrainTexture, String& path) {
    StringBuilder sb;
    {
        String str;
        sb.format("%sdiffuse.jpg", path.getValue());
        str = sb.toString();
        terrainTexture.diffuse = GlobalTextureLoader.loadRGBATileTexture(str);
        str.free();
    }

    sb.clear();
    {
        sb.format("%sspecular.jpg", path.getValue());
        String str = sb.toString();
        terrainTexture.specular = GlobalTextureLoader.loadRGBATileTexture(str);
        str.free();
    }

    sb.clear();
    {
        sb.format("%snormal.jpg", path.getValue());
        String str = sb.toString();
        terrainTexture.normal = GlobalTextureLoader.loadRGBATileTexture(str);
        str.free();
    }

    sb.free();
}

const char* pathToTextures = "./assets/terrain/textures/";
void Terrain::loadTextures(const GenerationParameters& params) {
    StringBuilder sb;
    String str;
    sb.format("%sgrass/", pathToTextures);
    str = sb.toString();
    loadTerrainTexture(textureList[0], str);
    sb.clear();

    sb.format("%ssand/", pathToTextures);
    str = sb.toString();
    loadTerrainTexture(textureList[1], str);
    sb.clear();

    sb.format("%ssnow/", pathToTextures);
    str = sb.toString();
    loadTerrainTexture(textureList[2], str);
    sb.clear();
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
        if (height > maxHeight * (1.f / 3.f)) {
            return TerrainClassification::DIRT;
        } else {
            return TerrainClassification::GRASS;
        }
    } else {
        if (height > maxHeight * (1.f / 3.f)) {
            return TerrainClassification::SNOW;
        } else {
            return TerrainClassification::GRASS;
        }
    }
}

void Terrain::initialize(const GenerationParameters& params) {
    if (isInitialized) {
        free();
    }

    logger_info("Loading textures for terrain...");
    loadTextures(params);

    logger_info("Generating terrain...");

    float squareSize = ((float)params.size / (float)params.granularity) / 2.f;
    int* perm = getSimplexArray(params.permSize);
    int permIndexCap = (params.permSize / 2) - 1;

    unsigned int numVertices = params.granularity * params.granularity;
    

	List<TerrainVertex> vertices;
	vertices.growDynamically = false;
	vertices.allocate(numVertices);

    const float portionOfTexturePerVertex = 1.f / (float)params.verticesPerTexture;
    Vector2f vertexCoordinates = { 0.f, 0.f };

	// Add all of the vertices
    for (int yCoord = 0; yCoord < params.granularity; yCoord++) {
        for (int xCoord = 0; xCoord < params.granularity; xCoord++) {
			float height = calculateSimplexValue(
				Vector2f { static_cast<GLfloat>(xCoord), static_cast<GLfloat>(yCoord) } + squareSize, params.minMaxHeight,
				params.scaleFactor, params.ampFactor, params.frequencyFactor,
				params.numOctaves, perm, permIndexCap);
			TerrainVertex vertex;
			vertex.position = getVec3(squareSize * xCoord, height, squareSize * yCoord);
			vertex.texCoords.x = vertexCoordinates.x;
			vertex.texCoords.y = vertexCoordinates.y;
			vertex.layer = static_cast<float>(classifyTerrain(height, params.minMaxHeight));
			vertices.add(vertex);
            vertexCoordinates.x += portionOfTexturePerVertex;
        }

        vertexCoordinates.x = 0.f;
        vertexCoordinates.y += portionOfTexturePerVertex;
    }

	// Add all of the indices
	numIndices = 6 * ((params.granularity - 1) * (params.granularity - 1));
	List<GLuint> indices;
	indices.growDynamically = false;
	indices.allocate(numIndices);
	for (int row = 0; row < params.granularity - 1; row++) {
		for (int col = 0; col < params.granularity - 1; col++) {
			GLuint vertexIdx = (row * params.granularity) + col;
			if (vertexIdx >= vertices.numElements) {
				printf("We're in trouble! row: %d, col: %d, %d\n", row, col, vertexIdx);
			}
			indices.add(vertexIdx);
			indices.add(vertexIdx + 1);
			indices.add(vertexIdx + params.granularity);
			indices.add(vertexIdx + params.granularity);
			indices.add(vertexIdx + params.granularity + 1);
			indices.add(vertexIdx + 1);
		}
	}

    // Calculate normals, tangents, and bitangents
    for (size_t indexIndex = 0; indexIndex < numIndices; indexIndex += 3) {
        if (indexIndex + 2 >= numIndices) {
            break;
        }

        GLint firstIndex = indices[indexIndex];
        GLint secondIndex = indices[indexIndex + 1];
		GLint thirdIndex = indices[indexIndex + 2];

        TerrainVertex& firstVertex = vertices[firstIndex];
        TerrainVertex& secondVertex = vertices[secondIndex];
        TerrainVertex& thirdVertex = vertices[thirdIndex];

        Vector3f normal = normalize(cross(firstVertex.position - secondVertex.position, 
		    thirdVertex.position - secondVertex.position));
        firstVertex.normal = normal;
        secondVertex.normal = normal;
        thirdVertex.normal = normal;

        // @TODO: This is copy and pasted in `main.cpp` of the ModelTransformTool.
        // Need to refactor that ASAP
        Vector3f deltaPosition1 = secondVertex.position - firstVertex.position;
        Vector3f deltaPosition2 = thirdVertex.position - firstVertex.position;

        Vector2f deltaUV1 = {
			secondVertex.texCoords.x - firstVertex.texCoords.x,
			secondVertex.texCoords.y - firstVertex.texCoords.y
		};
		Vector2f deltaUV2 = {
			thirdVertex.texCoords.x - firstVertex.texCoords.x,
			thirdVertex.texCoords.y - firstVertex.texCoords.y
		};

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

    logger_info("Finished generating terrain!");

    float halfMapSize = squareSize * (params.granularity / 2);
	model = Matrix4x4f { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
    model = translateMatrix(model, getVec3(-halfMapSize, 0, -halfMapSize));

	// Now, let's generate the OpenGL buffers needed here
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	glGenBuffers(1, &mEbo);

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(TerrainVertex), vertices.data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices.data, GL_STATIC_DRAW);
	
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)0);

	// Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, normal));
	
	// Tangent
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, tangent));

	// Bitangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, bitangent));

	// Texture Coords
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, texCoords));

	// Layer
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, layer));

	glBindVertexArray(0);

	vertices.deallocate();
	indices.deallocate();

    mParams = params;
    isInitialized = true;
}

void Terrain::render(const Camera* camera, const LightSystem* lightSystem) const {
    if (!isInitialized) {
        return;
    }

	useShader(ShaderUniformMapping::GlobalTerrainShaderMapping.shader);
	camera->render(ShaderUniformMapping::GlobalTerrainShaderMapping.cameraUniformMapping);
	setShaderMat4(ShaderUniformMapping::GlobalTerrainShaderMapping.UNIFORM_MODEL, model);

	lightSystem->render(&ShaderUniformMapping::GlobalTerrainShaderMapping.lightUniformMapping);

	for (int textureIndex = 0; textureIndex < 3; textureIndex++) {
	    setShaderInt(ShaderUniformMapping::GlobalTerrainShaderMapping.UNIFORM_DIFFUSE[textureIndex], textureIndex);
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, textureList[textureIndex].diffuse);

		setShaderInt(ShaderUniformMapping::GlobalTerrainShaderMapping.UNIFORM_SPECULAR[textureIndex], (textureIndex + 3));
		glActiveTexture(GL_TEXTURE0 + (textureIndex + 3));
		glBindTexture(GL_TEXTURE_2D, textureList[textureIndex].specular);

		setShaderInt(ShaderUniformMapping::GlobalTerrainShaderMapping.UNIFORM_NORMAL[textureIndex], (textureIndex + 6));
		glActiveTexture(GL_TEXTURE0 + (textureIndex + 6));
		glBindTexture(GL_TEXTURE_2D, textureList[textureIndex].normal);
	}

	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Terrain::free() {
    isInitialized = false;

	for (int idx = 0; idx < 3; idx++) {
		glDeleteTextures(1, &textureList[idx].diffuse);
		textureList[idx].diffuse = 0;
		glDeleteTextures(1, &textureList[idx].specular);
		textureList[idx].specular = 0;
		glDeleteTextures(1, &textureList[idx].normal);
		textureList[idx].normal = 0;
	}
	
	if (mVao) glDeleteVertexArrays(1, &mVao);
	if (mVbo) glDeleteBuffers(1, &mVbo);
    if (mEbo) glDeleteBuffers(1, &mEbo);

	mVao = 0;
	mVbo = 0;
	mEbo = 0;
}
