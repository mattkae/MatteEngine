#pragma once
#include <cstdlib>
#include <cstring>

template <typename T>
struct List {
	T* data = nullptr;
	size_t capacity = 0;
	size_t numElements = 0;

	void allocate(size_t size);
	void add(T* element);
	void grow(size_t newSize);
	void free();
	T* getValue(int index) const;
	T& operator[](int idx) const; 
};

template <typename T>
void List<T>::allocate(size_t size) {
	if (data != nullptr) {
		delete data;
	}

	data = new T[size];
	capacity = size;
	numElements = 0;
}

template <typename T>
void List<T>::grow(size_t newSize) {
	T* newData = new T[newSize];

	if (data != nullptr) {
		memcpy(newData, this->data, numElements * sizeof(T));
		delete this->data;
	}

	this->data = newData;
	this->capacity = newSize;
}

template <typename T>
void List<T>::add(T* element) {
	if (element == nullptr || data == nullptr) {
		return;
	}

	size_t newNumElements = this->numElements + 1;
	if (newNumElements == this->capacity) {
		grow(2 * this->capacity);
	}

	memcpy(&this->data[this->numElements], element, sizeof(T));
	this->numElements = newNumElements;
}

template <typename T>
void List<T>::free() {
	if (this->data != nullptr) {
		delete this->data;
		this->data = nullptr;
	}

	this->capacity = 0;
	this->numElements = 0;
}

template <typename T>
T* List<T>::getValue(int idx) const {
	return &this->data[idx];
}

template <typename T>
T& List<T>::operator[](int idx) const {
	return this->data[idx];
}