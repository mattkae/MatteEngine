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
	Matrix4x4f result = matrix;
	result.values[0] *= scale;
	result.values[5] *= scale;
	result.values[10] *= scale;
	return result;
}

Matrix4x4f mult(const Matrix4x4f& first, const Matrix4x4f& second) {
	/*Matrix4x4f result;
	for (int i = 0; i < 4; ++i) { 
        for (int j = 0; j < 4; ++j) { 
			int row = i * 4;
            result.values[row + j] = first.values[row + 0] * second.values[0 + j] + 
                         first.values[row + 1] * second.values[4 + j] + 
                         first.values[row + 2] * second.values[8 + j] + 
                         first.values[row + 3] * second.values[12 + j]; 
        } 
    } 

	return result;*/

	return {
		{
			// First row
			first.values[0] * second.values[0] + first.values[1] * second.values[4] + first.values[2] * second.values[8] + first.values[3] * second.values[12],
			first.values[0] * second.values[1] + first.values[1] * second.values[5] + first.values[2] * second.values[9] + first.values[3] * second.values[13],
			first.values[0] * second.values[2] + first.values[1] * second.values[6] + first.values[2] * second.values[10] + first.values[3] * second.values[14],
			first.values[0] * second.values[3] + first.values[1] * second.values[7] + first.values[2] * second.values[11] + first.values[3] * second.values[15],
			// Second row
			first.values[4] * second.values[0] + first.values[5] * second.values[4] + first.values[6] * second.values[8] + first.values[7] * second.values[12],
			first.values[4] * second.values[1] + first.values[5] * second.values[5] + first.values[6] * second.values[9] + first.values[7] * second.values[13],
			first.values[4] * second.values[2] + first.values[5] * second.values[6] + first.values[6] * second.values[10] + first.values[7] * second.values[14],
			first.values[4] * second.values[3] + first.values[5] * second.values[7] + first.values[6] * second.values[11] + first.values[7] * second.values[15],
			// Third row
			first.values[8] * second.values[0] + first.values[9] * second.values[4] + first.values[10] * second.values[8] + first.values[11] * second.values[12],
			first.values[8] * second.values[1] + first.values[9] * second.values[5] + first.values[10] * second.values[9] + first.values[11] * second.values[13],
			first.values[8] * second.values[2] + first.values[9] * second.values[6] + first.values[10] * second.values[10] + first.values[11] * second.values[14],
			first.values[8] * second.values[3] + first.values[9] * second.values[7] + first.values[10] * second.values[11] + first.values[11] * second.values[15],
			// Fourth row
			first.values[12] * second.values[0] + first.values[13] * second.values[4] + first.values[14] * second.values[8] + first.values[15] * second.values[12],
			first.values[12] * second.values[1] + first.values[13] * second.values[5] + first.values[14] * second.values[9] + first.values[15] * second.values[13],
			first.values[12] * second.values[2] + first.values[13] * second.values[6] + first.values[14] * second.values[10] + first.values[15] * second.values[14],
			first.values[12] * second.values[3] + first.values[13] * second.values[7] + first.values[14] * second.values[11] + first.values[15] * second.values[15],
		}
	};
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
		{
			1, 0, 0, 0,
			0, cos(angleRadians), sin(angleRadians), 0,
			0, -sin(angleRadians), cos(angleRadians), 0,
			0, 0, 0, 1
		}
	};
}

Matrix4x4f getYRotationMatrix(GLfloat angleRadians) {
	return {
		{
			cos(angleRadians), 0, -sin(angleRadians), 0,
			0, 1, 0, 0,
			sin(angleRadians), 0, cos(angleRadians), 0,
			0, 0, 0, 1
		}
	};
}

Matrix4x4f getZRotationMatrix(GLfloat angleRadians) {
	return {
		{
			cos(angleRadians), sin(angleRadians), 0, 0
			-sin(angleRadians), cos(angleRadians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		}
	};
}

Matrix4x4f rotate(const Matrix4x4f matrix, GLfloat xRadians, GLfloat yRadians, GLfloat zRadians) {
	Matrix4x4f result = matrix;
	Matrix4x4f rotationMatrix;
	if (xRadians != 0) {
		rotationMatrix = getXRotationMatrix(xRadians);
		result = mult(result, rotationMatrix);
	}

	if (yRadians != 0) {
		rotationMatrix = getYRotationMatrix(xRadians);
		result = mult(result, rotationMatrix);
	}

	if (zRadians != 0) {
		rotationMatrix = getZRotationMatrix(xRadians);
		result = mult(result, rotationMatrix);
	}

	return result;
}

Matrix4x4f translateMatrix(const Matrix4x4f& matrix, const Vector3f& translate) {
	Matrix4x4f result = matrix;
	result.values[12] += translate.x;
	result.values[13] += translate.y;
	result.values[14] += translate.z;
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
		{
			xAxis.x, yAxis.x, -zAxis.x, 0,
			xAxis.y, yAxis.y, -zAxis.y, 0,
			xAxis.z, yAxis.z, -zAxis.z, 0,
			-dotProduct(xAxis, eye), -dotProduct(yAxis, eye), dotProduct(zAxis, eye), 1
		}
	};
}

Matrix4x4f getProjection(GLfloat near,  GLfloat far, GLfloat fieldOfViewRadians, GLfloat aspectRatio) {
	const GLfloat halfFieldOfView = fieldOfViewRadians * 0.5f;
	const GLfloat top = tan(halfFieldOfView) * near;
	const GLfloat bottom = -top;
	const GLfloat right = top * aspectRatio;
	const GLfloat left = -right;

	return {
		{
			(2 * near) / (right - left), 0, 0 ,0,
			0, (2 * near) / (top - bottom), 0, 0,
			(right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near) , -1,
			0, 0,  (-2 * far * near) / (far - near), 0
		}
	};
}