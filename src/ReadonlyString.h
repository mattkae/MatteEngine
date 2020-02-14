#pragma once
#include <cstring>

struct ReadonlyString {
	char* data = nullptr;
	size_t length = 0;
	size_t bufferSize = 0;
	char* ptr = nullptr;

	void allocate(size_t bufferSize) {
		data = new char[bufferSize];
		bufferSize = bufferSize;
	}

	void grow(size_t newBufferSize) {
		if (newBufferSize <= bufferSize || data == nullptr) {
			return;
		}

		char* newData = new char[newBufferSize];
		memcpy(newData, data, bufferSize);
		delete[] data;
		data = newData;
		bufferSize = newBufferSize;
	}

	void free() {
		if (data != nullptr) {
			delete[] data;
		}
	}

	void print() {
		if (data != nullptr) {
			printf("%s", data);
		}
	}

	bool startsWith(const char* str, int length = -1) {
		if (length < 0) {
			length = strlen(str);
		}

		bool retval = true;

		size_t index = 0;
		while (retval && str[index] != '\0' && data[index] != '\0') {
			retval &= (str[index] == data[index]);
			index++;
		}

		return retval;
	}
};