#pragma once

#include "Model.h"
#include "Material.h"
#include "ObjModel.h"
#include "Logger.h"
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <vector>

/*
    Why write this myself? First of all, I want to know what's happening.
    But second of all, I want to be able to only parse the information that I need,
    thus speeding up the entire loading time. Also, in this way I'll be able to
    transform the file directly into my native types, rather than going
    through Assimp's intermediary types. And - lastly - I want to see if I can
    do it.
*/

std::vector<Material> loadMtlFile(std::string path) {
    std::vector<Material> materials;
    Logger::logInfo("Loading MTL file from " + path);

    std::ifstream mtlFile(path);
    if (!mtlFile.is_open()) {
        Logger::logError("Failed to parse file " + path);
        return materials;
    }

    Material current;

    for (std::string line; std::getline(mtlFile, line); !mtlFile.eof()) {
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
            iss >> current.ambient.r >> current.ambient.g >> current.ambient.b;
        } else if (type == "Kd") {
            iss >> current.diffuse.r >> current.diffuse.g >> current.diffuse.b;
        } else if (type == "Ks") {
            iss >> current.specular.r >> current.specular.g >> current.specular.b;
        } else if (type == "Ns") {
            iss >> current.specularComponent;
        } else if (type == "d") {
            iss >> current.dissolved;
        }

        // @TODO: Texture maps
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
    std::vector<FaceVertex> vertices;
};

Model loadFromObj(std::string path)
{
    Model model;
    Logger::logInfo("Loading OBJ file from " + path);

    std::ifstream objFile(path);
    if (!objFile.is_open()) {
        Logger::logError("Failed to parse file " + path);
        return;
    }

    std::vector<Material> materials;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> textureCoords;
    std::vector<Face> faces;
    bool useSmoothShading = false;

    std::string activeMaterialName;

    for (std::string line; std::getline(objFile, line); !objFile.eof()) {
        std::istringstream iss(line);
        std::string type;
        if (!(iss >> type)) {
            continue;
        }

        if (type == "#") {
            continue;
        } else if (type == "v") {
            // Found a vertex
            glm::vec3 point;
            iss >> point.x >> point.y >> point.z;
            vertices.push_back(point);
        } else if (type == "vn") {
            // Found a vertex normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (type == "vt") {
            // Found a vertex texture
            glm::vec3 coord;
            iss >> coord.x >> coord.y;

            if (!(iss >> coord.z)) {
                coord.z = 0;
            }

            textureCoords.push_back(coord);
        } else if (type == "f") {
            // Found a face
            Face face;
            std::string faceVertexString;
            const std::string delimiter = "/";
            while (iss >> faceVertexString) {
                FaceVertex vertex;
                int index = 0;
                size_t pos = 0;
                std::string faceVertexValueString;
                while ((pos = faceVertexString.find(delimiter)) != std::string::npos) {
                    faceVertexValueString = faceVertexString.substr(0, pos);
                    faceVertexString.erase(0, pos + delimiter.length());

                    if (faceVertexValueString.length() == 0) {
                        continue;
                    }

                    int faceVertexValue = std::stoi(faceVertexValueString);
                    switch (index) {
                        case 0:
                            vertex.vertexIndex = faceVertexValue;
                            break;
                        case 1:
                            vertex.vertexNormal = faceVertexValue;
                            break;
                        case 2: 
                            vertex.vertexTexture = faceVertexValue;
                            break;
                        default:
                            Logger::logError("Came accross invalid index while loading FaceVertex: " + index);
                            break;
                    }

                    index++;
                }

                face.vertices.push_back(vertex);
            }

            faces.push_back(face);
        } else if (type == "g") {
            // Found a group name
            std::string name;
            iss >> name;
        } else if (type == "o") {
            // Named object
        } else if (type == "usemtl") {
            // Using the specified material file
            iss >> activeMaterialName;
        } else if (type == "mtllib") {
            // @TODO Use filesystem from the std lib
            string directory;
            const size_t last_slash_idx = path.rfind('/');
            if (std::string::npos != last_slash_idx)
            {
                directory = path.substr(0, last_slash_idx);
            }
            std::string materialFile;
            iss >> materialFile;
            materials = loadMtlFile(directory + "/" + materialFile);
        } else if (type == "s") {
            // Specify smooth shading
            iss >> useSmoothShading;
        }
    }

    if (objFile.bad()) {
        Logger::logError("Error while reading OBJ file from " + path);
        return;
    }
}