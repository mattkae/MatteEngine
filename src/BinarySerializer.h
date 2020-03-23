#pragma once
#include <string>
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"

enum SerializationMode {
	READ = 0,
	WRITE
};

class BinarySerializer {
public:
	BinarySerializer(const char* file, SerializationMode mode);
	void close();

	void writeInt32(int value);
	void writeInt64(long value);
	void writeByte(char value);
	void writeFloat32(float value);
	void writeFloat64(double value);
	void writeString(std::string value);
	void writeVec2(Vector2f& v);
	void writeVec3(Vector3f& v);
	void writeVec4(Vector4f& v);

	int readInt32();
	long readInt64();
	char readByte();
	float readFloat32();
	double readFloat64();
	std::string readString();
	Vector2f readVec2();
	Vector3f readVec3();
	Vector4f readVec4();
private:
	const char* mFilePath;
	SerializationMode mMode;
	FILE* mFile;
	char buffer[16];
};