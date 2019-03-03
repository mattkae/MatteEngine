#include "Sphere.h"
#include "Shader.h"
#include "Vertex.h"
#include "GlmUtility.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

void to_json(nlohmann::json& j, const Sphere& sphere) {
    j = {
		{"radius", sphere.radius},
		{"transform", glm::mat4ToArray(sphere.model)}
    };
}

void from_json(const nlohmann::json& j, Sphere& sphere) {
    float angleIncrements = 5.0f;
    
    j.at("radius").get_to(sphere.radius);
    glm::from_json(j, "transform", sphere.model);
    j.at("angleIncrements").get_to(angleIncrements);
    sphere.generate(angleIncrements);
}

void Sphere::generate(float angleIncrements) {
    std::vector<Vertex> vertices;
    std::vector<GLint> indices;

    // Generate vertices and indices
    GLint index = 0;
    for (float phi = 0.0; phi <= 180; phi += angleIncrements) {
        const auto cosPhi = cos(glm::radians(phi));
        const auto sinPhi = sin(glm::radians(phi));

        const auto nextCosPhi = cos(glm::radians(phi + angleIncrements));
        const auto nextSinPhi = sin(glm::radians(phi + angleIncrements));

        for (float theta = 0.0; theta <= 360; theta += angleIncrements) {
            const auto cosTheta = cos(glm::radians(theta));
            const auto sinTheta = sin(glm::radians(theta));

            const auto nextSinTheta =
                sin(glm::radians(theta + angleIncrements));
            const auto nextCosTheta =
                cos(glm::radians(theta + angleIncrements));

            // Top Left Point
            auto topLeftPoint =
                glm::vec3(radius * sinPhi * cosTheta,
                          radius * sinPhi * sinTheta, radius * cosPhi);
            auto topLeftIdx = index++;
            vertices.push_back({topLeftPoint, glm::normalize(topLeftPoint)});

            // Bottom Left Point
            auto bottomLeftPoint =
                glm::vec3(radius * nextSinPhi * cosTheta,
                          radius * nextSinPhi * sinTheta, radius * nextCosPhi);
            auto bottomLeftIdx = index++;
            vertices.push_back(
                {bottomLeftPoint, glm::normalize(bottomLeftPoint)});

            // Bottom Right Point
            auto bottomRightPoint = glm::vec3(
                radius * nextSinPhi * nextCosTheta,
                radius * nextSinPhi * nextSinTheta, radius * nextCosPhi);
            auto bottomRightIdx = index++;
            vertices.push_back(
                {bottomRightPoint, glm::normalize(bottomRightPoint)});

            // Top Right Point
            auto topRightPoint =
                glm::vec3(radius * sinPhi * nextCosTheta,
                          radius * sinPhi * nextSinTheta, radius * cosPhi);
            auto topRightIdx = index++;
            vertices.push_back({topRightPoint, glm::normalize(topRightPoint)});

            indices.push_back(topLeftIdx);
            indices.push_back(bottomLeftIdx);
            indices.push_back(bottomRightIdx);
            indices.push_back(bottomRightIdx);
            indices.push_back(topLeftIdx);
            indices.push_back(topRightIdx);
        }
    }

    mMesh.mVertices = vertices;
    mMesh.mIndices = indices;
    mMesh.generate();
}

void Sphere::render(const Shader &shader, bool withMaterial) const {
    shader.set_uniform_matrix_4fv("uModel", 1, GL_FALSE, glm::value_ptr(model));
    mMesh.render(shader, withMaterial);
}

void Sphere::free() { mMesh.free_resources(); }
