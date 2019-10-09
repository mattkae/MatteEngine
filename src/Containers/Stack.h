#pragma once
template <typename T>
class Stack {
public:
	Stack();
	Stack(size_t defaultSize = 10);
	void push(T value);
	T pop();
	void free();
private:
	T* mList;
	size_t mSize = 0;
	size_t mPtr = 0;
};
