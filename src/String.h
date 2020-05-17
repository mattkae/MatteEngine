#pragma once

struct StringPointer {
	bool error = false;
	char* value = nullptr;
	size_t length = 0;
};

struct String {
	char* value = nullptr;
	size_t length = 0;
	size_t capacity = 0;

	void operator =(const char* str);
	void set(const char* str);
	void allocate(size_t capacity = 0);
	void deallocate();
	void print();
	void grow(size_t minNewCapacity = 0);
	void growIfSmaller(size_t newLength);
	void append(const char& c);
	void prepend(const char& c);
	void append(const String& other);
	void prepend(const String& other);
	void insert(const char& c, size_t index);
	void erase(size_t index);
	StringPointer substring(size_t start, size_t end);

	void fromInteger(int i);
	void fromFloat(float f);
	int toInteger();
	float toFloat();
};

