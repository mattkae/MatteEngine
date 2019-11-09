#pragma once

template <typename T>
struct LightweightArray {
	T* elements = nullptr;
	size_t size = 0;
};

template <typename T>
void allocateArray(LightweightArray<T>& arr, size_t size) {
	arr.elements = new T[size];
	arr.size = size;
}

template <typename T>
void deallocateArray(LightweightArray<T>& arr) {
	if (arr.elements != nullptr) {
		delete []arr.elements;
		arr.size = 0;
		arr.elements = nullptr;
	}
}