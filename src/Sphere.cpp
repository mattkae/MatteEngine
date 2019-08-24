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
    j.at("radius").get_to(sphere.radius);
    glm::from_json(j, "transform", sphere.model);
    j.at("angleIncrements").get_to(sphere.angleIncrements);
    initializeSphere(sphere);
}

void initializeSphere(Sphere& sphere) {
    std::vector<Vertex> vertices;
    std::vector<GLint> indices;

    // Generate vertices and indices
    GLint index = 0;
    for (float phi = 0.0; phi <= 180; phi += sphere.angleIncrements) {
        const auto cosPhi = cos(glm::radians(phi));
        const auto sinPhi = sin(glm::radians(phi));

        const auto nextCosPhi = cos(glm::radians(phi + sphere.angleIncrements));
        const auto nextSinPhi = sin(glm::radians(phi + sphere.angleIncrements));

        for (float theta = 0.0; theta <= 360; theta += sphere.angleIncrements) {
            const auto cosTheta = cos(glm::radians(theta));
            const auto sinTheta = sin(glm::radians(theta));

            const auto nextSinTheta = sin(glm::radians(theta + sphere.angleIncrements));
            const auto nextCosTheta =  cos(glm::radians(theta + sphere.angleIncrements));

            // Top Left Point
            auto topLeftPoint = glm::vec3(sphere.radius * sinPhi * cosTheta, sphere.radius * sinPhi * sinTheta, sphere.radius * cosPhi);
            auto topLeftIdx = index++;
            vertices.push_back({topLeftPoint, glm::normalize(topLeftPoint)});

            // Bottom Left Point
            auto bottomLeftPoint = glm::vec3(sphere.radius * nextSinPhi * cosTheta, sphere.radius * nextSinPhi * sinTheta, sphere.radius * nextCosPhi);
            auto bottomLeftIdx = index++;
            vertices.push_back({bottomLeftPoint, glm::normalize(bottomLeftPoint)});

            // Bottom Right Point
            auto bottomRightPoint = glm::vec3(sphere.radius * nextSinPhi * nextCosTheta, sphere.radius * nextSinPhi * nextSinTheta, sphere.radius * nextCosPhi);
            auto bottomRightIdx = index++;
            vertices.push_back({bottomRightPoint, glm::normalize(bottomRightPoint)});

            // Top Right Point
            auto topRightPoint = glm::vec3(sphere.radius * sinPhi * nextCosTheta,sphere.radius * sinPhi * nextSinTheta, sphere.radius * cosPhi);
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

    sphere.mesh.vertices = vertices;
    sphere.mesh.indicies = indices;
    initializeMesh(sphere.mesh);
}

void renderSphere(const Sphere& sphere, const Shader& shader, bool withMaterial) {
    shader.setMat4("uModel", sphere.model);
	renderMesh(sphere.mesh, shader, withMaterial);
}

void freeSphere(Sphere& sphere) {
	freeMesh(sphere.mesh);
}
