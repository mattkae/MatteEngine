#include "Matrix4x4f.h"

constexpr Matrix4x4f getIdentity() {
    Matrix4x4f result;
    result.values[0] = 1.f;
    result.values[5] = 1.f;
    result.values[10] = 1.f;
    result.values[15] = 1.f;
    return result;
}

Matrix4x4f scaleMatrix(const Matrix4x4f& matrix, const GLfloat scale) {
    Matrix4x4f result = copyMatrix(matrix);
    result.values[0] *= scale;
    result.values[5] *= scale;
    result.values[10] *= scale;
    return result;
}

Matrix4x4f setScale(const Matrix4x4f& matrix, const Vector3f& scale) {
    Matrix4x4f result = copyMatrix(matrix);
    result.values[0] = scale.x;
    result.values[5] = scale.y;
    result.values[10] = scale.z;
    return result;
}

Matrix4x4f setScale(const Matrix4x4f& matrix, const GLfloat scale) {
    Matrix4x4f result = copyMatrix(matrix);
    result.values[0] = scale;
    result.values[5] = scale;
    result.values[10] = scale;
    return result;
}

Matrix4x4f scaleMatrix(const Matrix4x4f& matrix, const Vector3f& scale) {
    Matrix4x4f result = copyMatrix(matrix);
    result.values[0] *= scale.x;
    result.values[5] *= scale.y;
    result.values[10] *= scale.z;
    return result;
}

Matrix4x4f mult(const Matrix4x4f& first, const Matrix4x4f& second) {
    Matrix4x4f result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int row = i * 4;
            result.values[row + j] = first.values[row + 0] * second.values[0 + j] + first.values[row + 1] * second.values[4 + j] + first.values[row + 2] * second.values[8 + j] + first.values[row + 3] * second.values[12 + j];
        }
    }

    return result;
}

Matrix4x4f getScalarMatrix(const GLfloat scale) {
    return scaleMatrix(getIdentity(), scale);
}

Vector4f mult(const Matrix4x4f& matrix, const Vector4f& vec) {
    return {
        vec.x * matrix.values[0] + vec.y * matrix.values[4] + vec.z * matrix.values[8] + vec.w * matrix.values[12],
        vec.x * matrix.values[1] + vec.y * matrix.values[5] + vec.z * matrix.values[9] + vec.w * matrix.values[13],
        vec.x * matrix.values[2] + vec.y * matrix.values[6] + vec.z * matrix.values[10] + vec.w * matrix.values[14],
        vec.x * matrix.values[3] + vec.y * matrix.values[7] + vec.z * matrix.values[11] + vec.w * matrix.values[15],
    };
}

Matrix4x4f getXRotationMatrix(GLfloat angleRadians) {
    return {
        { 1, 0, 0, 0,
            0, cos(angleRadians), -sin(angleRadians), 0,
            0, sin(angleRadians), cos(angleRadians), 0,
            0, 0, 0, 1 }
    };
}

Matrix4x4f getYRotationMatrix(GLfloat angleRadians) {
    return {
        { cos(angleRadians), 0, sin(angleRadians), 0,
            0, 1, 0, 0,
            -sin(angleRadians), 0, cos(angleRadians), 0,
            0, 0, 0, 1 }
    };
}

Matrix4x4f getZRotationMatrix(GLfloat angleRadians) {
    return {
        { cos(angleRadians), -sin(angleRadians), 0, 0,
            sin(angleRadians), cos(angleRadians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1 }
    };
}

Matrix4x4f rotate(const Matrix4x4f matrix, GLfloat xRadians, GLfloat yRadians, GLfloat zRadians) {
    Matrix4x4f result = copyMatrix(matrix);
    ;
    Matrix4x4f rotationMatrix;
    if (xRadians != 0) {
        rotationMatrix = getXRotationMatrix(xRadians);
        result = mult(result, rotationMatrix);
    }

    if (yRadians != 0) {
        rotationMatrix = getYRotationMatrix(yRadians);
        result = mult(result, rotationMatrix);
    }

    if (zRadians != 0) {
        rotationMatrix = getZRotationMatrix(zRadians);
        result = mult(result, rotationMatrix);
    }

    return result;
}

Matrix4x4f translateMatrix(const Matrix4x4f& matrix, const Vector3f& translate) {
    Matrix4x4f result = copyMatrix(matrix);
    result.values[12] += translate.x;
    result.values[13] += translate.y;
    result.values[14] += translate.z;
    return result;
}

Matrix4x4f setTranslation(const Matrix4x4f& matrix, const Vector3f& translate) {
    Matrix4x4f result = copyMatrix(matrix);
    result.values[12] = translate.x;
    result.values[13] = translate.y;
    result.values[14] = translate.z;
    return result;
}

Matrix4x4f translateMatrixX(const Matrix4x4f& matrix, float translate) {
	Matrix4x4f result = copyMatrix(matrix);;
	result.values[12] += translate;
	return result;
}

Matrix4x4f translateMatrixY(const Matrix4x4f& matrix, float translate) {
	Matrix4x4f result = copyMatrix(matrix);;
	result.values[13] += translate;
	return result;
}

Matrix4x4f translateMatrixZ(const Matrix4x4f& matrix, float translate) {
	Matrix4x4f result = copyMatrix(matrix);;
	result.values[14] += translate;
	return result;
}

Matrix4x4f transpose(const Matrix4x4f& matrix) {
    Matrix4x4f result;

    for (int row = 0; row < DIM_OF_MAT; row++) {
        for (int col = 0; col < DIM_OF_MAT; col++) {
            result.values[row * DIM_OF_MAT + col] = matrix.values[col * DIM_OF_MAT + row];
        }
    }

    return result;
}

// See https://stackoverflow.com/questions/349050/calculating-a-lookat-matrix for why the dot product is in the bottom
Matrix4x4f getLookAt(const Vector3f& eye, const Vector3f& pointToLookAt, const Vector3f& up) {
    Vector3f zAxis = normalize(subtractVector(pointToLookAt, eye));
    Vector3f xAxis = normalize(cross(zAxis, up));
    Vector3f yAxis = normalize(cross(xAxis, zAxis));

    return {
        { xAxis.x, yAxis.x, -zAxis.x, 0,
            xAxis.y, yAxis.y, -zAxis.y, 0,
            xAxis.z, yAxis.z, -zAxis.z, 0,
            -dotProduct(xAxis, eye), -dotProduct(yAxis, eye), dotProduct(zAxis, eye), 1 }
    };
}

Matrix4x4f getPerspectiveProjection(GLfloat near, GLfloat far, GLfloat fieldOfViewRadians, GLfloat aspectRatio) {
    const GLfloat halfFieldOfView = fieldOfViewRadians * 0.5f;
    const GLfloat top = tan(halfFieldOfView) * near;
    const GLfloat bottom = -top;
    const GLfloat right = top * aspectRatio;
    const GLfloat left = -right;

    return {
        { (2 * near) / (right - left), 0, 0, 0,
            0, (2 * near) / (top - bottom), 0, 0,
            (right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near), -1,
            0, 0, (-2 * far * near) / (far - near), 0 }
    };
}

Matrix4x4f getOrthographicProjection(GLfloat near, GLfloat far, GLfloat left, GLfloat right, GLfloat top, GLfloat bottom) {
	Matrix4x4f result;
	result.values[0] = 2.f / (right - left);
	result.values[5] = -2.f / (top - bottom);
	result.values[10] = -2.f / (far - near);
	result.values[12] = -(right + left) / (right - left);
	result.values[13] = -(top + bottom) / (top - bottom);
	result.values[14] = -(far + near) / (far - near);
	return result;
}

Matrix4x4f getOrthographicProjection(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top) {
	Matrix4x4f result;
	result.values[0] = 2.f / (right - left);
	result.values[5] = 2.f / (top - bottom);
	result.values[10] = 1.f;
	result.values[12] = -(right + left) / (right - left);
	result.values[13] = -(top + bottom) / (top - bottom);
	return result;
}

// Honestly, this is too much of a pain to write myself. I stole it from here:
// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
bool inverse(const Matrix4x4f& m, Matrix4x4f& result) {
    float inv[16], det;
    int i;

    inv[0] = m.values[5] * m.values[10] * m.values[15] - m.values[5] * m.values[11] * m.values[14] - m.values[9] * m.values[6] * m.values[15] + m.values[9] * m.values[7] * m.values[14] + m.values[13] * m.values[6] * m.values[11] - m.values[13] * m.values[7] * m.values[10];

    inv[4] = -m.values[4] * m.values[10] * m.values[15] + m.values[4] * m.values[11] * m.values[14] + m.values[8] * m.values[6] * m.values[15] - m.values[8] * m.values[7] * m.values[14] - m.values[12] * m.values[6] * m.values[11] + m.values[12] * m.values[7] * m.values[10];

    inv[8] = m.values[4] * m.values[9] * m.values[15] - m.values[4] * m.values[11] * m.values[13] - m.values[8] * m.values[5] * m.values[15] + m.values[8] * m.values[7] * m.values[13] + m.values[12] * m.values[5] * m.values[11] - m.values[12] * m.values[7] * m.values[9];

    inv[12] = -m.values[4] * m.values[9] * m.values[14] + m.values[4] * m.values[10] * m.values[13] + m.values[8] * m.values[5] * m.values[14] - m.values[8] * m.values[6] * m.values[13] - m.values[12] * m.values[5] * m.values[10] + m.values[12] * m.values[6] * m.values[9];

    inv[1] = -m.values[1] * m.values[10] * m.values[15] + m.values[1] * m.values[11] * m.values[14] + m.values[9] * m.values[2] * m.values[15] - m.values[9] * m.values[3] * m.values[14] - m.values[13] * m.values[2] * m.values[11] + m.values[13] * m.values[3] * m.values[10];

    inv[5] = m.values[0] * m.values[10] * m.values[15] - m.values[0] * m.values[11] * m.values[14] - m.values[8] * m.values[2] * m.values[15] + m.values[8] * m.values[3] * m.values[14] + m.values[12] * m.values[2] * m.values[11] - m.values[12] * m.values[3] * m.values[10];

    inv[9] = -m.values[0] * m.values[9] * m.values[15] + m.values[0] * m.values[11] * m.values[13] + m.values[8] * m.values[1] * m.values[15] - m.values[8] * m.values[3] * m.values[13] - m.values[12] * m.values[1] * m.values[11] + m.values[12] * m.values[3] * m.values[9];

    inv[13] = m.values[0] * m.values[9] * m.values[14] - m.values[0] * m.values[10] * m.values[13] - m.values[8] * m.values[1] * m.values[14] + m.values[8] * m.values[2] * m.values[13] + m.values[12] * m.values[1] * m.values[10] - m.values[12] * m.values[2] * m.values[9];

    inv[2] = m.values[1] * m.values[6] * m.values[15] - m.values[1] * m.values[7] * m.values[14] - m.values[5] * m.values[2] * m.values[15] + m.values[5] * m.values[3] * m.values[14] + m.values[13] * m.values[2] * m.values[7] - m.values[13] * m.values[3] * m.values[6];

    inv[6] = -m.values[0] * m.values[6] * m.values[15] + m.values[0] * m.values[7] * m.values[14] + m.values[4] * m.values[2] * m.values[15] - m.values[4] * m.values[3] * m.values[14] - m.values[12] * m.values[2] * m.values[7] + m.values[12] * m.values[3] * m.values[6];

    inv[10] = m.values[0] * m.values[5] * m.values[15] - m.values[0] * m.values[7] * m.values[13] - m.values[4] * m.values[1] * m.values[15] + m.values[4] * m.values[3] * m.values[13] + m.values[12] * m.values[1] * m.values[7] - m.values[12] * m.values[3] * m.values[5];

    inv[14] = -m.values[0] * m.values[5] * m.values[14] + m.values[0] * m.values[6] * m.values[13] + m.values[4] * m.values[1] * m.values[14] - m.values[4] * m.values[2] * m.values[13] - m.values[12] * m.values[1] * m.values[6] + m.values[12] * m.values[2] * m.values[5];

    inv[3] = -m.values[1] * m.values[6] * m.values[11] + m.values[1] * m.values[7] * m.values[10] + m.values[5] * m.values[2] * m.values[11] - m.values[5] * m.values[3] * m.values[10] - m.values[9] * m.values[2] * m.values[7] + m.values[9] * m.values[3] * m.values[6];

    inv[7] = m.values[0] * m.values[6] * m.values[11] - m.values[0] * m.values[7] * m.values[10] - m.values[4] * m.values[2] * m.values[11] + m.values[4] * m.values[3] * m.values[10] + m.values[8] * m.values[2] * m.values[7] - m.values[8] * m.values[3] * m.values[6];

    inv[11] = -m.values[0] * m.values[5] * m.values[11] + m.values[0] * m.values[7] * m.values[9] + m.values[4] * m.values[1] * m.values[11] - m.values[4] * m.values[3] * m.values[9] - m.values[8] * m.values[1] * m.values[7] + m.values[8] * m.values[3] * m.values[5];

    inv[15] = m.values[0] * m.values[5] * m.values[10] - m.values[0] * m.values[6] * m.values[9] - m.values[4] * m.values[1] * m.values[10] + m.values[4] * m.values[2] * m.values[9] + m.values[8] * m.values[1] * m.values[6] - m.values[8] * m.values[2] * m.values[5];

    det = m.values[0] * inv[0] + m.values[1] * inv[4] + m.values[2] * inv[8] + m.values[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.f / det;

    for (i = 0; i < 16; i++)
        result.values[i] = inv[i] * det;

    return true;
}