#include "ObjModel.h"
#include "Logger.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ObjModel.h"
#include "Vertex.h"
#include "StringUtil.h"
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <SOIL.h>

/*
    Why write this myself? First of all, I want to know what's happening.
    But second of all, I want to be able to only parse the information that I need,
    thus speeding up the entire loading time. Also, in this way I'll be able to
    transform the file directly into my native types, rather than going
    through Assimp's intermediary types. And - lastly - I want to see if I can
    do it.
*/

GLuint loadTexture(std::string path){
	GLuint texture;
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	const char* cStrPath = path.c_str();
    unsigned char* image = SOIL_load_image(cStrPath, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == NULL) {
		Logger::logError("Unable to load image from path: " + path);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture;
}

inline std::string getDirectory(std::string strPath) {
	std::string directory;
    const size_t last_slash_idx = strPath.rfind('/');
    if (std::string::npos != last_slash_idx) {
        directory = strPath.substr(0, last_slash_idx);
	}
    return directory;       
}

std::vector<Material> loadMtlFile(std::string path) {
    std::vector<Material> materials;
    Logger::logInfo("Loading MTL file from " + path);

    std::ifstream mtlFile(path);
    if (!mtlFile.is_open()) {
        Logger::logError("Failed to parse file " + path);
        return materials;
    }

    Material current;

    for (std::string line; std::getline(mtlFile, line);) {
        std::istringstream iss(line);
        std::string type;
        if (!(iss >> type)) {
            continue;
        }

        if (type == "newmtl") {
            if (!current.name.empty()) {
                materials.push_back(current);
            }
            iss >> current.name;
        } else if (type == "Ka") {
            iss >> current.ambient.x >> current.ambient.y >> current.ambient.z;
        } else if (type == "Kd") {
            iss >> current.diffuse.x >> current.diffuse.y >> current.diffuse.z;
        } else if (type == "Ks") {
            iss >> current.specular.x >> current.specular.y >> current.specular.z;
        } else if (type == "Ns") {
            iss >> current.specularComponent;
        } else if (type == "d") {
            iss >> current.transparency;
        } else if (type == "map_Ks") {
			std::string filename;
			iss >> filename;
			current.specularTexture = loadTexture(getDirectory(path) +  "/" + filename);
		} else if (type == "map_Ka") {
			std::string filename;
			iss >> filename;
			current.ambientTexture = loadTexture(getDirectory(path) +  "/" + filename);
		} else if (type == "map_Kd") {
			std::string filename;
			iss >> filename;
			current.diffuseTexture = loadTexture(getDirectory(path) +  "/" + filename);
		}
    }

	if (!current.name.empty()) {
        materials.push_back(current);
    }

    if (mtlFile.bad()) {
        Logger::logError("Error while reading OBJ file from " + path);
        return materials;
    }

    return materials;
}

struct FaceVertex {
    int vertexIndex = -1;
    int vertexTexture = -1;
    int vertexNormal = -1;
};

struct Face {
    std::string mtlName;
    std::vector<FaceVertex> vertices;
};

inline int getLengthToNextFaceVal(char* ptr) {
	int lToSlash = StringUtil::getLengthToChar(ptr, '/');
	int lToEnd = StringUtil::getLengthToChar(ptr, '\0');
	if (lToSlash == 0) {
		return lToEnd;
	} else if (lToEnd == 0) {
		return lToSlash;
	} else {
		return std::min(lToEnd, lToSlash);
	}
}

void loadFromObj(char* filepath, Model& model) {
	    FILE* file;
#ifdef __APPLE__
    file = fopen(filepath, "r");
#else
    fopen_s(&file, filepath, "r");
#endif

	char buffer[StringUtil::DEFAULT_BUFFER_SIZE];
    if (file == NULL) {
        // @TODO: Error out
        return;
    }

	// @TODO: This can be optimized further in the future
	std::vector<Material> materials;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;
    std::vector<Face> faces;
    bool useSmoothShading = false;
	std::string activeMaterialName;

	char* ptr;
	while (StringUtil::processLine(file, buffer, ptr)) {
		if (StringUtil::startsWith(ptr, "#")) {
			continue;
		} else if (StringUtil::ifEqualWalkToValue(ptr, "vt")) {
            glm::vec2 coord;
			StringUtil::strToVec2(ptr, coord);
			coord.y = 1.f - coord.y;
            textureCoords.push_back(coord);
        } else if (StringUtil::ifEqualWalkToValue(ptr, "vn")) {
			glm::vec3 normal;
			StringUtil::strToVec3(ptr, normal);
			normals.push_back(normal);
		} else if (StringUtil::ifEqualWalkToValue(ptr, "v")) {
			glm::vec3 vertex;
			StringUtil::strToVec3(ptr, vertex);
			vertices.push_back(vertex);
		} else if (StringUtil::ifEqualWalkToValue(ptr, "f")) {
			Face face;
            face.mtlName = activeMaterialName;

			char faceBuffer[StringUtil::DEFAULT_BUFFER_SIZE];
			char tokenBuffer[StringUtil::DEFAULT_BUFFER_SIZE];

			while (ptr[0] != '\0') {
				// Get the next token represented by x/y/z
				int lengthToNextSpace = StringUtil::getLengthToChar(ptr, ' ');
				int lengthToEnd = StringUtil::getLengthToChar(ptr, '\0');
				StringUtil::substring(tokenBuffer, ptr, std::min(lengthToNextSpace, lengthToEnd));

				char* tokenBuffPtr = tokenBuffer;
				FaceVertex vertex;
				int lengthToDelim = getLengthToNextFaceVal(tokenBuffPtr);
				int index = 0;
				int faceVertexValue;
				while (true) {
					// Extract the value
					StringUtil::substring(faceBuffer, tokenBuffPtr, lengthToDelim);
					StringUtil::strToInt(faceBuffer, faceVertexValue);
					faceVertexValue= faceVertexValue - 1;

					// Set the value
					switch (index) {
					case 0:
						vertex.vertexIndex = faceVertexValue;
						break;
					case 1:
						vertex.vertexTexture = faceVertexValue;
						break;
					case 2:
						vertex.vertexNormal = faceVertexValue;
						break;
					default:
						Logger::logError("Came accross invalid index while loading FaceVertex: " + index);
						break;
					}

					// Advance the ptr
					tokenBuffPtr += lengthToDelim;
					ptr += lengthToDelim;

					// End the madness, if we're at the end of the line
					if (tokenBuffPtr[0] == '\0') {
						break;
					} else if (tokenBuffPtr[0] == '/') {
						tokenBuffPtr++;
						ptr++;

						if (tokenBuffPtr[0] == '/') {
							index++;
							tokenBuffPtr++;
							ptr++;
						}
					}

					lengthToDelim = getLengthToNextFaceVal(tokenBuffPtr);
					index++;
				}

				face.vertices.push_back(vertex);
				if (ptr[0] != '\0') {
					ptr++;
				}
			}
			faces.push_back(face);
		} else if (StringUtil::ifEqualWalkToValue(ptr, "g")) {
			// @TODO: Get the name
		} else if (StringUtil::ifEqualWalkToValue(ptr, "o")) {
			// @TODO: Get name of object
		} else if (StringUtil::ifEqualWalkToValue(ptr, "usemtl")) {
			char activeMatrialCharPtr[StringUtil::DEFAULT_BUFFER_SIZE];
			size_t activeMaterialCharPtrLen = StringUtil::getLengthToEndOfLine(ptr);
			StringUtil::substring(activeMatrialCharPtr, ptr, activeMaterialCharPtrLen);
			activeMaterialName = std::string(activeMatrialCharPtr);
		} else if (StringUtil::ifEqualWalkToValue(ptr, "s")) {
			StringUtil::strToBool(ptr, useSmoothShading);
		} else if (StringUtil::ifEqualWalkToValue(ptr, "mtllib")) {
			char materialFileNameChar[StringUtil::DEFAULT_BUFFER_SIZE];
			StringUtil::substring(materialFileNameChar, ptr, StringUtil::getLengthToEndOfLine(ptr));
			materials = loadMtlFile(getDirectory(std::string(filepath)) + "/" + std::string(materialFileNameChar));
		}
	}

	fclose(file);

	std::vector<Mesh> meshes;
    for (Face& face : faces) {
        Mesh mesh;
        std::vector<Vertex> meshVertices;
        std::vector<GLint> indices;

        for (size_t vertexIndex = 0; vertexIndex < face.vertices.size(); vertexIndex++) {
            FaceVertex faceVertex = face.vertices.at(vertexIndex);
            Vertex vertex;
            if (faceVertex.vertexNormal > -1) {        
	            vertex.normal = normals.at(faceVertex.vertexNormal);
			}
			if (faceVertex.vertexTexture >  -1) {
				vertex.texCoords = textureCoords.at(faceVertex.vertexTexture);
			}
            if (faceVertex.vertexIndex > -1) {
				vertex.position = vertices.at(faceVertex.vertexIndex);
            }

			if (vertexIndex > 2 && vertexIndex % 3 == 0) {
                indices.push_back(vertexIndex - 3);
                indices.push_back(vertexIndex - 1);
			}

            meshVertices.push_back(vertex);
            indices.push_back(vertexIndex);
        }

        mesh.vertices = meshVertices;
        mesh.indicies = indices;

		for (auto& material : materials) {
            if (material.name == face.mtlName) {
				mesh.material = material;
                break;
			}
		}

        meshes.push_back(mesh);
    }

	model.meshes = meshes;
}