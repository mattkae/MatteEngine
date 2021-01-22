#include "MyString.h"
#include "MathHelper.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstdarg>
#include <cmath>

int StringBuffer::add(const char* str) {
	int spaceNeeded = strlen(str);
	int spaceAvailable = BUFFER_SIZE - pointer;

	int amountToCopy = 0;
	if (spaceAvailable >= spaceNeeded) {
		amountToCopy = spaceNeeded;
	} else {
		amountToCopy = spaceAvailable;
	}

	memcpy(&buffer[pointer], str, sizeof(char) * amountToCopy);
	pointer += amountToCopy;
	buffer[pointer] = '\0';
	return amountToCopy;
}

void StringBuffer::reset() {
	buffer[0] = '\0';
	pointer = 0;	
}

bool StringBuffer::isFull() {
	return pointer == BUFFER_SIZE;
}

StringBuffer* StringBuilder::getCurrentBuffer() {
	if (bufferPointer == 0) {
		if (!defaultBuffer.isFull()) {
			return &defaultBuffer;
		}

		StringBuffer newBuffer;
		dynamicBuffer.allocate(2);
		bufferPointer++;
		dynamicBuffer.add(&newBuffer);
	}

	if (dynamicBuffer[bufferPointer - 1].isFull()) {
		StringBuffer newBuffer;
		dynamicBuffer.add(&newBuffer);
		bufferPointer++;
	}

	return &dynamicBuffer[bufferPointer - 1];
}

StringBuffer* StringBuilder::getBufferAtIdx(int index) {
	if (index == 0) {
		return &defaultBuffer;
	}

	index = index - 1;
	if (index < static_cast<int>(dynamicBuffer.numElements)) {
		return &dynamicBuffer[index];
	}

	return nullptr;
}

const StringBuffer* StringBuilder::getBufferAtIdxConst(int index) const {
	if (index == 0) {
		return &defaultBuffer;
	}

	index = index - 1;
	if (index < static_cast<int>(dynamicBuffer.numElements)) {
		return &dynamicBuffer[index];
	}

	return nullptr;
}

String StringBuilder::toString() {
	int strSize = (bufferPointer + 1) * StringBuffer::BUFFER_SIZE;
	String retval;

	if (strSize > String::SSO_SIZE) {
		retval.dynamicBuffer = new char[strSize + 1];
		memcpy(retval.dynamicBuffer, defaultBuffer.buffer, sizeof(char) * StringBuffer::BUFFER_SIZE);
		FOREACH(dynamicBuffer) {
			memcpy(&retval.dynamicBuffer[(idx + 1) * StringBuffer::BUFFER_SIZE], value->buffer, sizeof(char) * StringBuffer::BUFFER_SIZE);
		}
		retval.isSSO = false;
	} else {
		memcpy(retval.defaultBuffer, defaultBuffer.buffer, sizeof(char) * strSize);
		retval.isSSO = true;
	}

	retval.capacity = strSize;
	retval.length = length;
	retval.getValue()[retval.length] = '\0';
	return retval;
}

void StringBuilder::copyTo(String* str) {
	int strSize = (bufferPointer + 1) * StringBuffer::BUFFER_SIZE;

	if (str == nullptr) {
		logger_error("String uninitialized");
		return;
	}

	if (strSize > String::SSO_SIZE) {
		if (str->capacity < strSize + 1) { // Needed to reallocate
			int newSize =  MathHelper::nearestPowerOfTwo(strSize + 1);
			str->free();
			str->dynamicBuffer = new char[newSize];
			str->capacity = newSize;
		}

		str->dynamicBuffer[0] = '\0';
		memcpy(&str->dynamicBuffer[0], defaultBuffer.buffer, sizeof(char) * defaultBuffer.pointer);
		FOREACH(dynamicBuffer) {
			memcpy(&str->dynamicBuffer[(idx + 1) * StringBuffer::BUFFER_SIZE], value->buffer, sizeof(char) * value->pointer);
		}
		str->isSSO = false;
	} else {
		memcpy(str->defaultBuffer, defaultBuffer.buffer, sizeof(char) * strSize);
		str->isSSO = true;
	}

	str->length = length;
	str->getValue()[str->length] = '\0';
}

void StringBuilder::addStr(String* str) {
	addStr(str->getValue());
}

void StringBuilder::addStr(const char* str) {
	int amountLeft = strlen(str);
	length += amountLeft;
	int ptr = 0;

	do {
		StringBuffer* currentBuffer = getCurrentBuffer();
		int amtAdded = currentBuffer->add(&str[ptr]);
		amountLeft = amountLeft - amtAdded;
		ptr += amtAdded;
	} while (amountLeft > 0);
}

void StringBuilder::addChar(char c) {
	char wrapper[2] = { c, '\0' };
	addStr(wrapper);
}

const int DEFAULT_NUMBER_BUFFER_SIZE = 32;

void StringBuilder::addInt(int value) {
	char buffer[DEFAULT_NUMBER_BUFFER_SIZE];
	sprintf(buffer, "%d", value);
	addStr(buffer);
}

// Pulled from here: https://stackoverflow.com/questions/7228438/convert-double-float-to-string
inline int n_tu(int number, int count) {
    int result = 1;
    while(count-- > 0)
        result *= number;

    return result;
}

void StringBuilder::addFloat(float value) {
	char buffer[DEFAULT_NUMBER_BUFFER_SIZE];
	sprintf(buffer, "%f", value);
	addStr(buffer);
}

void StringBuilder::replace(const char* strToReplace, const char* replaceStr) {
	
}

int StringBuilder::indexOf(const char* str) {
	return -1;
}

void StringBuilder::removeAt(int index, int count) {
	if (index >= length) {
		logger_error("Index is larger than the length");
		return;
	}

	if (count <= 0) {
		printf("Count was <= zero");
		return;
	}

	int newLength = length - count;
	if (newLength == 0) {
		clear();
		return;
	}

	// Easiest way to do this is to move the entire string to the left, starting at index by count
	int currentIndex = index;
	int copyIndex = index + count;

	while (currentIndex < newLength) {
		int bufferPos = floor(static_cast<float>(currentIndex) / static_cast<float>(StringBuffer::BUFFER_SIZE));
		int pointerIdx = currentIndex % StringBuffer::BUFFER_SIZE;
		StringBuffer* buffer = getBufferAtIdx(bufferPos);

		char newChar = '\0';
		int copyBufferPos = floor(static_cast<float>(copyIndex) / static_cast<float>(StringBuffer::BUFFER_SIZE));
		StringBuffer* copyBuffer = getBufferAtIdx(copyBufferPos);
		if (copyBuffer != nullptr) {
			int bufferPointerIdx = copyIndex % StringBuffer::BUFFER_SIZE;
			if (bufferPointerIdx < copyBuffer->pointer) {
				newChar = copyBuffer->buffer[bufferPointerIdx];
			}
		}

		buffer->buffer[pointerIdx] = newChar;

		currentIndex++;
		copyIndex++;
	}

	// Brute force update the buffer that we're currently using
	length = newLength;
	int newNumBuffers = ceil(static_cast<float>(length) / static_cast<float>(StringBuffer::BUFFER_SIZE)); // This is how many buffers we should have
	if (newNumBuffers <= 1) {
		dynamicBuffer.numElements = 0;
		bufferPointer = 0;
	} else {
		dynamicBuffer.numElements = newNumBuffers - 1;
		bufferPointer = newNumBuffers - 1;
	}

	StringBuffer* lastBuffer = getBufferAtIdx(bufferPointer);
	if (lastBuffer->pointer != StringBuffer::BUFFER_SIZE) {
		lastBuffer->pointer = length % StringBuffer::BUFFER_SIZE;
	}
	lastBuffer->buffer[lastBuffer->pointer] = '\0';
}

void StringBuilder::format(const char* str, ...) {
	va_list args;
    va_start(args, str);

	while (*str != '\0') {
		if (*str == '%') {
			str++;
			switch (*str) {
			case 'd':
				addInt(va_arg(args, int));
				break;
			case 'f':
				addFloat(static_cast<float>(va_arg(args, double)));
				break;
			case 's':
				addStr(va_arg(args, char*));
			default:
				break;
			}
		} else {
			addChar(*str);
		}

		str++;
	}

	va_end(args);
}

void StringBuilder::clear() {
	bufferPointer = 0;
	length = 0;
	defaultBuffer.reset();

	if (!dynamicBuffer.isEmpty()) {
		FOREACH(dynamicBuffer) {
			value->reset();
		}
	}

	dynamicBuffer.clear();
}

void StringBuilder::free() {
	dynamicBuffer.deallocate();
}

char StringBuilder::getCharAtIdx(int index) const {
	int bufferPos = floor(static_cast<float>(index) / static_cast<float>(StringBuffer::BUFFER_SIZE));
	int pointerIdx = index % StringBuffer::BUFFER_SIZE;
	const StringBuffer* buffer = getBufferAtIdxConst(bufferPos);
	if (buffer != nullptr && pointerIdx < buffer->pointer) {
		return buffer->buffer[pointerIdx];
	}

	return '\0';
}

void StringBuilder::insert(char c, int index) {
	if (index >= length) {
		addChar(c);
		return;
	}

	// Move all of the characters forward by one
	char currentChar = c;
	for (int cIdx = index; cIdx < length + 1; cIdx++) {
		int bufferPos = floor(static_cast<float>(cIdx) / static_cast<float>(StringBuffer::BUFFER_SIZE));
		int pointerIdx = cIdx % StringBuffer::BUFFER_SIZE;
		StringBuffer* buffer = getBufferAtIdx(bufferPos);
		if (buffer == nullptr) {
			buffer = getCurrentBuffer(); // Get a new buffer if this one is filled up
		}

		char nextCurrent = buffer->buffer[pointerIdx];
		buffer->buffer[pointerIdx] = currentChar;
		currentChar = nextCurrent;
	}

	// Move the final buffer forward
	StringBuffer* finalBuffer = getCurrentBuffer();
	finalBuffer->pointer++;
	finalBuffer->buffer[finalBuffer->pointer] = '\0';
	length = length + 1;
}

String::String() {
}

String::String(const char* str) {
	set(str);
}

void String::operator =(const char* str) {
	set(str);
}

char* String::getValue() {
	return isSSO ? defaultBuffer : dynamicBuffer;
}

const char* String::getValueConst() const {
	return isSSO ? defaultBuffer : dynamicBuffer;
}

void String::set(const char* str) {
	int inLen = strlen(str);

	if (inLen <= SSO_SIZE) {
		isSSO = true;
		memcpy(defaultBuffer, str, inLen);
		defaultBuffer[inLen] = '\0';
	} else {
		if (capacity >= inLen) {
			memcpy(dynamicBuffer, str, inLen);
		} else {
			free();
			capacity = MathHelper::nearestPowerOfTwo(inLen + 1);
			dynamicBuffer = new char[capacity];
			memcpy(dynamicBuffer, str, inLen);
		}
		isSSO = false;
		dynamicBuffer[inLen] = '\0';
	}

	length = inLen;
}

void String::free() {
	capacity = 0;
	length = 0;
	isSSO = true;
	if (dynamicBuffer != nullptr) {
		delete dynamicBuffer;
		dynamicBuffer = nullptr;
	}

	defaultBuffer[0] = '\0';
}

int String::toInteger() {
	return atoi(getValue());
}

float String::toFloat() {
	return static_cast<float>(atof(getValue()));
}

int String::indexOf(char c) {
	char* buffer = getValue();
	int idx = 0;
	while (buffer[idx] != '\0') {
		if (buffer[idx] == c) {
			return idx;
		}
		idx++;
	}

	return -1;
}

StringView String::substring(int start, int end) {
	StringView retval;
	if (start >= length) {
		retval.error = true;
		return retval;
	}

	if (end >= length) {
		end = length - 1;
	}

	char* value = getValue();
	retval.value = &value[start];
	retval.length = end - start;
	return retval;
}
