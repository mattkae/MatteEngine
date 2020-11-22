#include "BinarySerializer.h"
#include "FileHelper.h"
#include "Logger.h"

void print_byte_as_bits(char val) {
	for (int i = 7; 0 <= i; i--) {
		printf("%c", (val & (1 << i)) ? '1' : '0');
	}
	printf("\n");
}

BinarySerializer::BinarySerializer(const char* file, SerializationMode mode) {
	mFilePath = file;
	mMode = mode;

	switch (mMode) {
	case SerializationMode::READ:
		mFile = FileHelper::openFile(file, "rb");
		break;
	case SerializationMode::WRITE:
		mFile = FileHelper::openFile(file, "wb");
		break;
	default:
		mFile = nullptr;
		break;
	}
}

void BinarySerializer::close() const {
	if (mFile != nullptr) {
		fclose(mFile);
	}
}

/*
	Writes
*/
void BinarySerializer::writeInt32(int value) const {
	if (mMode != SerializationMode::WRITE) {
		Logger::error("Cannot write binary, wrong mode");
		return;
	}

	fwrite(&value, sizeof(int), 1, mFile);
}

void BinarySerializer::writeUint32(unsigned int value) const {
	if (mMode != SerializationMode::WRITE) {
		Logger::error("Cannot write binary, wrong mode");
		return;
	}

	fwrite(&value, sizeof(unsigned int), 1, mFile);
}

void BinarySerializer::writeByte(char value) const {
	if (mMode != SerializationMode::WRITE) {
		Logger::error("Cannot write binary, wrong mode");
		return;
	}

	fwrite(&value, sizeof(char), 1, mFile);
}

void BinarySerializer::writeInt64(long value) const {
	if (mMode != SerializationMode::WRITE) {
		Logger::error("Cannot write binary, wrong mode");
		return;
	}

	fwrite(&value, sizeof(long), 1, mFile);
}

void BinarySerializer::writeFloat32(float value) const {
	if (mMode != SerializationMode::WRITE) {
		Logger::error("Cannot write binary, wrong mode");
		return;
	}

	fwrite(&value, sizeof(float), 1, mFile);
}

void BinarySerializer::writeFloat64(double value) const {
	if (mMode != SerializationMode::WRITE) {
		Logger::error("Cannot write binary, wrong mode");
		return;
	}

	fwrite(&value, sizeof(value), 1, mFile);
}

void BinarySerializer::writeString(std::string value) const {
	if (mMode != SerializationMode::WRITE) {
		Logger::error("Cannot write binary, wrong mode");
		return;
	}

	writeInt32(value.size());
	for (size_t charIndex = 0; charIndex < value.size(); charIndex++) {
		writeByte(value[charIndex]);
	}
}

void BinarySerializer::writeVec2(Vector2f& v) const {
	writeFloat32(v.x);	
	writeFloat32(v.y);	
}

void BinarySerializer::writeVec3(Vector3f& v) const {
	writeFloat32(v.x);	
	writeFloat32(v.y);	
	writeFloat32(v.z);	
}

void BinarySerializer::writeVec4(Vector4f& v) const {
	writeFloat32(v.x);	
	writeFloat32(v.y);	
	writeFloat32(v.z);	
	writeFloat32(v.w);	
}

void BinarySerializer::writeMat4x4(Matrix4x4f& m) const {
	for (unsigned int index = 0; index < SIZE_OF_4_X_4; index++) {
		writeFloat32(m.values[index]);
	}
}

void BinarySerializer::writeQuaternion(Quaternion& q) const {
	writeFloat32(q.w);	
	writeFloat32(q.x);	
	writeFloat32(q.y);	
	writeFloat32(q.z);	
}

/*
	Reads
*/
int BinarySerializer::readInt32() {
	int retval;
	fread(buffer, sizeof(int), 1, mFile);
	memcpy(&retval, buffer, sizeof(int));
	return retval;
}

long BinarySerializer::readInt64() {
	long retval;
	fread(buffer, sizeof(long), 1, mFile);
	memcpy(&retval, buffer, sizeof(long));
	return retval;
}

unsigned int BinarySerializer::readUint32() {
	unsigned int retval;
	fread(buffer, sizeof(unsigned int), 1, mFile);
	memcpy(&retval, buffer, sizeof(unsigned int));
	return retval;
}

char BinarySerializer::readByte() {
	fread(buffer, sizeof(char), 1, mFile);
	return buffer[0];
}

float BinarySerializer::readFloat32() {
	float retval;
	fread(buffer, sizeof(float), 1, mFile);
	memcpy(&retval, buffer, sizeof(float));
	return retval;
}

double BinarySerializer::readFloat64() {
	double retval;
	fread(buffer, sizeof(double), 1, mFile);
	memcpy(&retval, buffer, sizeof(double));
	return retval;
}

std::string BinarySerializer::readString() {
	int size = readInt32();
	std::string retval;
	retval.resize(size);
	for (int index = 0; index < size; index++) {
		retval[index] = readByte();
	}
	return retval;
}

Vector2f BinarySerializer::readVec2() {
	Vector2f retval;
	retval.x = readFloat32();
	retval.y = readFloat32();
	return retval;
}

Vector3f BinarySerializer::readVec3() {
	Vector3f retval;
	retval.x = readFloat32();
	retval.y = readFloat32();
	retval.z = readFloat32();
	return retval;
}

Vector4f BinarySerializer::readVec4() {
	Vector4f retval;
	retval.x = readFloat32();
	retval.y = readFloat32();
	retval.z = readFloat32();
	retval.w = readFloat32();
	return retval;
}

Matrix4x4f BinarySerializer::readMat4x4() {
	Matrix4x4f retval;
	for (unsigned int index = 0; index < SIZE_OF_4_X_4; index++) {
		retval.values[index] = readFloat32();
	}
	return retval;
}

Quaternion BinarySerializer::readQuaternion() {
	Quaternion retval;
	retval.w = readFloat32();
	retval.x = readFloat32();
	retval.y = readFloat32();
	retval.z = readFloat32();
	return retval;
}