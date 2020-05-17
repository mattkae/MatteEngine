#include "String.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

const size_t defaultSize = 32;

void String::set(const char* str) {
	size_t newLength = strlen(str);
	if (newLength > capacity) {
		grow(newLength);
	}

	memcpy(value, str, newLength);
	length = newLength;
}

void String::operator =(const char* str) {
	set(str);
}

void String::allocate(size_t capacity) {
	if (capacity == 0) {
		capacity = defaultSize;
	}

	this->capacity = capacity;
	this->length = 0;
	value = static_cast<char*>(malloc(sizeof(char) * capacity));
	if (value != nullptr && capacity > 0) {
		value[0] = '\0';
	}
}

void String::deallocate() {
	if (value != nullptr) {
		free(value);
		capacity = 0;
		length = 0;
	}
}

void String::grow(size_t minNewCapacity) {
	if (capacity == 0) {
		capacity = defaultSize;
	}

	size_t newCapacity = 2 * capacity;
	if (newCapacity < minNewCapacity) {
		newCapacity = minNewCapacity * 2;
	}

	char* newValue = static_cast<char*>(malloc(sizeof(char) * newCapacity));
	if (newValue == nullptr) {
		// @TODO Log error
		return;
	}

	if (value != nullptr) {
		if (length > 0) {
			memcpy(newValue, value, sizeof(char) * length);
		}
		free(value);
	}

	value = newValue;
	this->capacity = newCapacity;
}

void String::print() {
	printf("%s\n", value);
}

void String::growIfSmaller(size_t newLength) {
	if (newLength > capacity) {
		grow(newLength);
	}
}

void String::append(const String& other) {
	size_t newLength = length + other.length;
	growIfSmaller(newLength);

	memcpy(&value[length], other.value, sizeof(char) * other.length);
	length = newLength;
	value[length] = '\0';
}

void String::append(const char& c) {
	size_t newLength = length + 1;
	growIfSmaller(newLength);

	value[length] = c;
	length = length + 1;
	value[length] = '\0';
}

void String::prepend(const char& c) {
	size_t newLength = length + 1;
	growIfSmaller(newLength);

	memmove(&value[1], value, length);
	value[0] = c;
	length = length + 1;
	value[length] = '\0';
}

void String::prepend(const String& other) {
	size_t newLength = length + other.length;
	growIfSmaller(newLength);

	memmove(&value[other.length], value, length); // Move current string to the end of the other
	memcpy(value, other.value, other.length - 1);
	length = newLength;
	value[length] = '\0';
}

void String::insert(const char& c, size_t index) {
	if (index == 0) {
		return prepend(c);
	}

	if (index == length) {
		return append(c);
	}

	size_t newLength = length + 1;
	growIfSmaller(newLength);
	memmove(&value[index + 1], &value[index], length - index);
	value[index] = c;
	length = length + 1;
	value[length] = '\0';
}

void String::erase(size_t index) {
	if (index >= length) {
		return;
	}

	memmove(&value[index], &value[index + 1], length - index + 1);
	length = length - 1;
}

StringPointer String::substring(size_t start, size_t end) {
	StringPointer retval;
	if ((start > end) || (start > length)) {
		retval.error = true;
		return retval;
	}

	if (end > length) {
		end = length;
	}

	retval.value = &value[start];
	retval.length = end - start;
	return retval;
}

void String::fromInteger(int i) {
	if (value == nullptr) {
		allocate();
	}

	snprintf(value, 10, "%d", i);
	length = strlen(value);
}

void String::fromFloat(float f) {
	if (value == nullptr) {
		allocate();
	}

	snprintf(value, 10, "%f", f);
	length = strlen(value);
}

int String::toInteger() {
	return atoi(value);
}

float String::toFloat() {
	return atof(value);
}