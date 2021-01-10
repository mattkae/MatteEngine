#include "Sphere.h"
#include "MathHelper.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Box.h"
#include "List.h"
#include <cmath>

void initializeSphere(const Sphere& sphere, Model& model, Box3D& box) {
    List<Vertex> vertices;
    List<GLint> indices;

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
            vertices.add({topLeftPoint, normalize(topLeftPoint)});

            // Bottom Left Point
            auto bottomLeftPoint = getVec3(sphere.radius * nextSinPhi * cosTheta, sphere.radius * nextSinPhi * sinTheta, sphere.radius * nextCosPhi);
            auto bottomLeftIdx = index++;
            vertices.add({bottomLeftPoint, normalize(bottomLeftPoint)});

            // Bottom Right Point
            auto bottomRightPoint = getVec3(sphere.radius * nextSinPhi * nextCosTheta, sphere.radius * nextSinPhi * nextSinTheta, sphere.radius * nextCosPhi);
            auto bottomRightIdx = index++;
            vertices.add({bottomRightPoint, normalize(bottomRightPoint)});

            // Top Right Point
            auto topRightPoint = getVec3(sphere.radius * sinPhi * nextCosTheta,sphere.radius * sinPhi * nextSinTheta, sphere.radius * cosPhi);
            auto topRightIdx = index++;
            vertices.add({topRightPoint, normalize(topRightPoint)});

            indices.add(topLeftIdx);
            indices.add(bottomLeftIdx);
            indices.add(bottomRightIdx);
            indices.add(bottomRightIdx);
            indices.add(topLeftIdx);
            indices.add(topRightIdx);
        }
    }

	model.mModel = sphere.model;
    model.meshes = new Mesh[1];
    model.numMeshes = 1;
    model.meshes[0].initializeFromVertices(&vertices, &indices);
}
