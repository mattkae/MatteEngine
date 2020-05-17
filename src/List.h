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
	void deallocate();
	T* getValue(int index) const;
	T& operator[](int idx) const; 
};

template <typename T>
void List<T>::allocate(size_t size) {
	if (data != nullptr) {
		deallocate();
	}

	data = static_cast<T*>(malloc(sizeof(T) * size));
	capacity = size;
	numElements = 0;
}

template <typename T>
void List<T>::grow(size_t newSize) {
	T* newData = static_cast<T*>(malloc(sizeof(T) * newSize));

	if (data != nullptr) {
		memcpy(newData, data, numElements * sizeof(T));
		delete data;
	}

	data = newData;
	capacity = newSize;
}

template <typename T>
void List<T>::add(T* element) {
	if (element == nullptr || data == nullptr) {
		return;
	}

	size_t newNumElements = numElements + 1;
	if (newNumElements > capacity) {
		grow(2 * capacity);
	}

	memcpy(&data[numElements], element, sizeof(T));
	numElements = newNumElements;
}

template <typename T>
void List<T>::deallocate() {
	if (data != nullptr) {
		free(data);
		data = nullptr;
	}

	capacity = 0;
	numElements = 0;
}

template <typename T>
T* List<T>::getValue(int idx) const {
	return &data[idx];
}

template <typename T>
T& List<T>::operator[](int idx) const {
	return data[idx];
}