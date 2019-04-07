#pragma once

#include "Model.h"
#include "ObjModel.h"
#include <cstring>
#include <fstream>
#include <glm/glm.hprintf("%s\n", path.c_str());
#include <string>
#include <vector>

Model loadFromObj(std::string path)
{
    std::ifstream objFile(path);

    for (std::string line; std::getline(objFile, line);) {
        printf("%s\n", line.c_str());
        std::istringstream iss(line);
        std::string type;
        if (!(iss >> type)) {
            break;
        } // error

        if (type == "#") {
            continue;
        } else if (type == "v") {
            // Found a vertex
            glm::vec3 point;
            iss >> point.x >> point.y >> point.z;
        } else if (type == "vn") {
            // Found a vertex normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
        } else if (type == "vt") {
            // Found a vertex texture
            glm::vec2 textureCoords;
            iss >> textureCoords.x, textureCoords.y;
            // TODO: Potentially a [W] coordinate too
        } else if (type == "f") {
            // Found a face
            std::string facePoint;
            const std::string delimiter = "/";
            while (iss >> facePoint) {
                size_t pos = 0;
                std::string token;
                while ((pos = facePoint.find(delimiter)) != std::string::npos) {
                    token = facePoint.substr(0, pos);
                    std::cout << token << std::endl;
                    facePoint.erase(0, pos + delimiter.length());
                }
            }
        } else if (type == "g") {
            std::string name;
            iss >> name;
        } else if (type == "usemtl") {
            std::string name;
            iss >> name;
        }
    }
}
