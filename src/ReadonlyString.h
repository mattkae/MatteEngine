#pragma once
#include <cstring>

struct ReadonlyStrPtr {
	size_t startIndex = 0;
	size_t length = 0;
};

struct ReadonlyString {
	// Info about the underlying string
	char* data = nullptr;
	size_t length = 0;
	size_t bufferSize = 0;

	// Info about where we're currently looking in the string
	char* ptr = nullptr;
	size_t ptrPosition = 0;

	void allocate(size_t bufferSize);
	void grow(size_t newBufferSize);
	void setStr(const char* str);
	void free();
	void print();
	bool startsWith(const char* str, int length = -1);
	void movePastWhiteSpace();
	void advancePtrPastChar(char delim);
	ReadonlyStrPtr createPointerToChar(char delim);
	ReadonlyStrPtr createPointerToCharArr(const char* delims);
	bool advancePtr(size_t ele = 1);
	bool isAtEnd();
};
