#pragma once

#define FOREACH_FIXED(fixedArray)												\
	for (size_t idx = 0; idx < fixedArray.mNumElements; idx++)					\
		if (auto value = fixedArray.getValue(idx)) \

#define FOREACH_FIXED_CONST(fixedArray)												\
	for (size_t idx = 0; idx < fixedArray.mNumElements; idx++)					\
		if (auto value = fixedArray.getValueConst(idx)) \
		

template <typename T, int S>
struct FixedArray {
	T data[S];
	int mNumElements = 0;

	void add(T& element) {
		data[mNumElements] = element;
		mNumElements++;
	}

	T* getValue(int index) {
		return &data[index];
	}

	const T* getValueConst(int index) const {
		return &data[index];
	}

	T& operator[](int idx) {
		return data[idx];
	}

	const T& operator[](int idx) const {
		return data[idx];
	}
};
