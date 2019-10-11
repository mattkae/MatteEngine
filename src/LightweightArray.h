#pragma once

template <typename T>
struct LightweightArray {
	T* elements = nullptr;
	size_t numElements = 0;
};

template <typename T>
void allocateArray(LightweightArray<T>& arr, size_t size) {
	arr.elements = new T[size];
	arr.numElements = size;
}

template <typename T>
void deallocateArray(LightweightArray<T>& arr) {
	if (arr.elements != nullptr) {
		delete arr.elements;
		arr.numElements = 0;
	}
}