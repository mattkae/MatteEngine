#pragma once
#include "Vector3f.h"
#include "Matrix4x4f.h"
#include <assimp/Importer.hpp>

Vector3f assimpColor4ToVec3(aiColor4D inColor) {
    return { inColor.r, inColor.g, inColor.b };
}

Vector3f assimpVec3ToVec3(aiVector3D v) {
    return { v.x, v.y, v.z };
}

Matrix4x4f assimpMatrixToMatrix(const aiMatrix4x4& matrix) {
    Matrix4x4f retval;

    retval.values[0] = matrix.a1;
    retval.values[1] = matrix.a2;
    retval.values[2] = matrix.a3;
    retval.values[3] = matrix.a4;

    retval.values[4] = matrix.b1;
    retval.values[5] = matrix.b2;
    retval.values[6] = matrix.b3;
    retval.values[7] = matrix.b4;

    retval.values[8] = matrix.c1;
    retval.values[9] = matrix.c2;
    retval.values[10] = matrix.c3;
    retval.values[11] = matrix.c4;

    retval.values[12] = matrix.d1;
    retval.values[13] = matrix.d2;
    retval.values[14] = matrix.d3;
    retval.values[15] = matrix.d4;

    return transpose(retval);
}