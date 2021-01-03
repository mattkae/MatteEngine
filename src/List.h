#pragma once
#include <cstdlib>
#include <cstring>
#include "Logger.h"

#define FOREACH(list)												\
	for (size_t idx = 0; idx < list.numElements; idx++) \
		if (auto value = list.getValue(idx)) \
		

template <typename T>
struct List {
	T* data = nullptr;
	size_t capacity = 0;
	size_t numElements = 0;
	bool growDynamically = true;

	void allocate(size_t size);
	void add(T* element);
	void add(T& element);
	void add(T&& element);
	bool grow(size_t newSize);
	void set(T* value, size_t index);
	void remove(size_t index);
	void clear();
	void deallocate();
	bool isEmpty() {
		return data == nullptr || numElements == 0;
	}
	T* getValue(int index) const;
	T& operator[](int idx) const;
	void binarySort(int (*f)(T *first, T* second));
	void setFromArray(T* arry, int size) {
	    allocate(size);
		memcpy(data, arry, size * sizeof(T));
		numElements = size;
	}
};

template <typename T>
void List<T>::allocate(size_t size) {
	if (size == 0 || size == capacity) {
		numElements = 0;
		return;
	}

	if (data != nullptr) {
		deallocate();
	}

	data = static_cast<T*>(malloc(sizeof(T) * size));
	capacity = size;
	numElements = 0;
}

template <typename T>
bool List<T>::grow(size_t newSize) {
	if (!growDynamically) {
		return false;
	}

	if (newSize == 0) {
		return false;
	}

	T* newData = static_cast<T*>(malloc(sizeof(T) * newSize));

	if (data != nullptr) {
		memcpy(newData, data, numElements * sizeof(T));
		delete data;
	}

	data = newData;
	capacity = newSize;
	return true;
}

template <typename T>
void List<T>::set(T* value, size_t index) {
	if (index >= capacity && !grow(index * 2)) {
		return;
	}

	memcpy(&data[index], value, sizeof(T));
}

template <typename T>
void List<T>::add(T* element) {
	if (data == nullptr) {
		allocate(2);
	}

	if (element == nullptr) {
		logger_error("Element not defined");
		return;
	}

	size_t newNumElements = numElements + 1;
	if (newNumElements > capacity) {
		if (!grow(2 * capacity)) {
			logger_error("Trying to add to list but unable to grow the array");
			return;
		}
	}

	memcpy(&data[numElements], element, sizeof(T));
	numElements = newNumElements;
}

template <typename T>
void List<T>::add(T& element) {
	if (data == nullptr) {
		allocate(2);
	}

	size_t newNumElements = numElements + 1;
	if (newNumElements > capacity) {
		if (!grow(2 * capacity)) {
			logger_error("Trying to add to list but unable to grow the array");
			return;
		}
	}

	memcpy(&data[numElements], &element, sizeof(T));
	numElements = newNumElements;
}

template <typename T>
void List<T>::add(T&& element) {
	if (data == nullptr) {
		allocate(2);
	}

	size_t newNumElements = numElements + 1;
	if (newNumElements > capacity) {
		if (!grow(2 * capacity)) {
			logger_error("Trying to add to list but unable to grow the array");
			return;
		}
	}

	memcpy(&data[numElements], &element, sizeof(T));
	numElements = newNumElements;
}

template <typename T>
void List<T>::remove(size_t idx) {
	if (idx >= numElements) {
		logger_error("Index is outside of the list: %d >= %d", idx, numElements);
		return;
	}

	for (; idx < numElements - 1; idx++) {
		data[idx] = data[idx + 1];
	}

	numElements--;
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
void List<T>::clear() {
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

template <typename T>
void List<T>::binarySort(int (*f)(T *first, T* second)) {
	if (data == nullptr) {
		return;
	}

	for (size_t idx = 0; idx < numElements - 1; idx++) {
		int minIdx = idx;
		T firstValue = data[idx];

		for (int innerIdx = idx + 1; innerIdx < numElements; innerIdx++) {\
			T secondValue = data[innerIdx];
			if (f(&firstValue, &secondValue) > 0) {
				minIdx= innerIdx;
			}
		}

		T temp = data[minIdx];
		memmove(&data[minIdx], &data[idx], sizeof(T));
		memmove(&data[idx], &temp, sizeof(T));
	}
}
