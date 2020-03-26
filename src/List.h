#pragma once

struct List {
	void* data = nullptr;
	size_t elementSize = 0;
	size_t capacity = 0;
	size_t numElements = 0;
	size_t sizeBytes = 0;

	void allocate(size_t size, size_t sizeOfElement);
	void add(void* element);
	void grow(size_t newSize);
	void deallocate();
	char* getValue(int index);
};