#pragma once
#include <string>
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Quaternion.h"
#include "Matrix4x4f.h"

enum SerializationMode {
	READ = 0,
	WRITE
};

class BinarySerializer {
public:
	BinarySerializer(const char* file, SerializationMode mode);
	void close() const;

	void writeInt32(int value) const;
	void writeInt64(long value) const;
	void writeUint32(unsigned int value) const;
	void writeByte(char value) const;
	void writeFloat32(float value) const;
	void writeFloat64(double value) const;
	void writeString(std::string value) const;
	void writeVec2(Vector2f& v) const;
	void writeVec3(Vector3f& v) const;
	void writeVec4(Vector4f& v) const;
	void writeMat4x4(Matrix4x4f& m) const;
	void writeQuaternion(Quaternion& m) const;

	int readInt32();
	long readInt64();
	unsigned int readUint32();
	char readByte();
	float readFloat32();
	double readFloat64();
	std::string readString();
	Vector2f readVec2();
	Vector3f readVec3();
	Vector4f readVec4();
	Matrix4x4f readMat4x4();
	Quaternion readQuaternion();
private:
	const char* mFilePath;
	SerializationMode mMode;
	FILE* mFile;
	char buffer[16];
};