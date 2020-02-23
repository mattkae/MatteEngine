#include "ReadonlyString.h"
#include <stdio.h>

void ReadonlyString::allocate(size_t newBufferSize) {
	data = new char[newBufferSize];
	bufferSize = newBufferSize;
	ptrPosition = 0;
	ptr = data;
	length = 0;
}

void ReadonlyString::grow(size_t newBufferSize) {
	if (newBufferSize <= bufferSize || data == nullptr) {
		return;
	}

	char* newData = new char[newBufferSize];
	memcpy(newData, data, bufferSize);
	delete[] data;
	data = newData;
	bufferSize = newBufferSize;
}

void ReadonlyString::setStr(const char* str) {
	size_t strSize = strlen(str);
	if (data == nullptr || bufferSize < strSize) {
		allocate(strSize + 1);
	}

	memcpy(data, str, strSize);
	data[strSize] = '\0';
	ptrPosition = 0;
	ptr = data;
	length = strSize;
}

void ReadonlyString::free() {
	if (data != nullptr) {
		delete[] data;
		bufferSize = 0;
		length = 0;
		ptr = nullptr;
		ptrPosition = 0;
	}
}

void ReadonlyString::print() {
	if (data != nullptr) {
		printf("%s", data);
	}
}

bool ReadonlyString::startsWith(const char* str, int length) {
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

void ReadonlyString::movePastWhiteSpace() {
	if (ptr == nullptr) {
		return;
	}

	while (ptr[0] == ' ') {
		advancePtr();
	}
}

bool ReadonlyString::advancePtr(size_t ele) {
	if (ptrPosition == length || ptr[0] == '\0') {
		return false;
	}

	ptrPosition += ele;
	ptr += ele;
	return true;
};

void ReadonlyString::advancePtrPastChar(char delim) {
	while (ptr[0] != delim) {
		advancePtr();
	}

	advancePtr();
}

ReadonlyStrPtr ReadonlyString::createPointerToChar(char delim) {
	char arrOfOne[1] = { delim };
	return createPointerToCharArr(arrOfOne);
}

ReadonlyStrPtr ReadonlyString::createPointerToCharArr(const char* delims) {
	ReadonlyStrPtr retval;

	retval.startIndex = ptrPosition;
	bool found = false;
	do {
		for (int dIdx = 0; dIdx < sizeof(delims); dIdx++) {
			if (ptr[0] == delims[dIdx]) {
				found = true;
				break;
			}
		}
	} while (!found && advancePtr());

	if (found) {
		retval.length = ptrPosition - retval.startIndex;
		advancePtr();
	} else {
		retval.length = 0;
	}

	return retval;
}

bool ReadonlyString::isAtEnd() {
	return ptrPosition >= length;
}
