#include "List.h"
#include <cstdlib>
#include <cstring>

void List::allocate(size_t size, size_t sizeOfElement) {
	size_t nextSizeBytes = size * sizeOfElement;
	if (data != nullptr) {
		// If we're already at the next size, don't reallocate, just change the size to zero
		if (sizeBytes >= nextSizeBytes) {
			this->elementSize = sizeOfElement;
			return;
		}
	}

	this->elementSize = sizeOfElement;
	this->data = malloc(nextSizeBytes);
	this->capacity = size;
	this->sizeBytes = nextSizeBytes;
}

void List::grow(size_t newSize) {
	if (data == nullptr) {
		allocate(newSize, this->elementSize);
		return;
	}

	size_t newSizeBytes = this->elementSize * newSize;
	void* newData = malloc(newSizeBytes);
	if (newData == nullptr) {
		return;
	}

	memcpy(newData, this->data, this->sizeBytes);
	free(this->data);
	this->data = newData;
	this->capacity = newSize;
	this->sizeBytes = newSizeBytes;
}

void List::add(void* element) {
	if (element == nullptr || data == nullptr) {
		return;
	}

	size_t newNumElements = this->numElements + 1;
	if (newNumElements == this->capacity) {
		grow(2 * this->capacity);
	}

	char* ptr = ((char*)this->data);
	memcpy(&ptr[this->numElements * this->elementSize], element, this->elementSize);
	this->numElements = newNumElements;
}

void List::deallocate() {
	if (this->data == nullptr) {
		return;
	}

	free(this->data);
	this->data = nullptr;
	this->capacity = 0;
	this->numElements = 0;
	this->elementSize = 0;
	this->sizeBytes = 0;
}