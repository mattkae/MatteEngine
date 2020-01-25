#include "Sphere.h"
#include "MathHelper.h"
#include "Vertex.h"
#include <cmath>
#include <vector>

void initializeSphere(const Sphere& sphere, Model& model, Box& box) {
    std::vector<Vertex> vertices;
    std::vector<GLint> indices;

    // Generate vertices and indices
    GLint index = 0;
    for (float phi = 0.0; phi <= 180; phi += sphere.angleIncrements) {
        const auto cosPhi = cos(MathHelper::degreesToRadians(phi));
        const auto sinPhi = sin(MathHelper::degreesToRadians(phi));

        const auto nextCosPhi = cos(MathHelper::degreesToRadians(phi + sphere.angleIncrements));
        const auto nextSinPhi = sin(MathHelper::degreesToRadians(phi + sphere.angleIncrements));

        for (float theta = 0.0; theta <= 360; theta += sphere.angleIncrements) {
            const auto cosTheta = cos(MathHelper::degreesToRadians(theta));
            const auto sinTheta = sin(MathHelper::degreesToRadians(theta));

            const auto nextSinTheta = sin(MathHelper::degreesToRadians(theta + sphere.angleIncrements));
            const auto nextCosTheta =  cos(MathHelper::degreesToRadians(theta + sphere.angleIncrements));

            // Top Left Point
            auto topLeftPoint = getVec3(sphere.radius * sinPhi * cosTheta, sphere.radius * sinPhi * sinTheta, sphere.radius * cosPhi);
            auto topLeftIdx = index++;
            vertices.push_back({topLeftPoint, normalize(topLeftPoint)});

            // Bottom Left Point
            auto bottomLeftPoint = getVec3(sphere.radius * nextSinPhi * cosTheta, sphere.radius * nextSinPhi * sinTheta, sphere.radius * nextCosPhi);
            auto bottomLeftIdx = index++;
            vertices.push_back({bottomLeftPoint, normalize(bottomLeftPoint)});

            // Bottom Right Point
            auto bottomRightPoint = getVec3(sphere.radius * nextSinPhi * nextCosTheta, sphere.radius * nextSinPhi * nextSinTheta, sphere.radius * nextCosPhi);
            auto bottomRightIdx = index++;
            vertices.push_back({bottomRightPoint, normalize(bottomRightPoint)});

            // Top Right Point
            auto topRightPoint = getVec3(sphere.radius * sinPhi * nextCosTheta,sphere.radius * sinPhi * nextSinTheta, sphere.radius * cosPhi);
            auto topRightIdx = index++;
            vertices.push_back({topRightPoint, normalize(topRightPoint)});

            indices.push_back(topLeftIdx);
            indices.push_back(bottomLeftIdx);
            indices.push_back(bottomRightIdx);
            indices.push_back(bottomRightIdx);
            indices.push_back(topLeftIdx);
            indices.push_back(topRightIdx);
        }
    }

	model.model = sphere.model;
	model.meshes.push_back({
		vertices,
		indices
	});
    initializeModel(model, box);
}