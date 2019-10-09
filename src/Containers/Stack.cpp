#include "Stack.h"

template <typename T>
Stack<T>::Stack() {
	this(10);
}

template <typename T>
Stack<T>::Stack(size_t defaultSize) {
	mSize = defaultSize;
	mList = new T[mSize];
}

template <typename T>
void Stack<T>::push(T value) {
	if (mPtr == mSize) {
		T newList[] = new T[mSize * 2];
		memcpy(newList, mList, mSize * sizeof(T));
		delete mList;
		mList = newList;
	}

	mList[mPtr++] = value;
}

template <typename T>
T Stack<T>::pop() {
	return mList[mPtr--]; 
}

template <typename T>
void Stack<T>::free() {
	if (mList) {
		delete mList;
	}
}