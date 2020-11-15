#pragma once
#include "List.h"

struct StringView {
	bool error = false;
	char* value = nullptr;
	size_t length = 0;
};

struct String {
	const static int SSO_SIZE = 31;

	char defaultBuffer[String::SSO_SIZE + 1] = { '\0' };
	char* dynamicBuffer = nullptr;

	int length = 0;
	int capacity = 0;
	bool isSSO = true;

	String();
	String(const char* str);
	char* getValue();
	const char* getValueConst() const;
	void operator =(const char* str);
	void set(const char* str);
	void free();
	int toInteger();
	float toFloat();
	int indexOf(char c);
	StringView substring(int start, int end);
};

struct StringBuffer {
	const static int BUFFER_SIZE = 31;

	int pointer = 0;
	char buffer[StringBuffer::BUFFER_SIZE + 1]; // Leave space for trailing escape character

	/* 
	* Appends the string to the buffer
	* @param str
	* @returns number of characters copied
	*/
	int add(const char* str);
	bool isFull();
	void reset();
};

struct StringBuilder {
	int bufferPointer = 0;
	int length = 0;

	StringBuffer defaultBuffer;
	List<StringBuffer> dynamicBuffer;

	StringBuffer* getCurrentBuffer();
	StringBuffer* getBufferAtIdx(int index);
	const StringBuffer* getBufferAtIdxConst(int index) const;
	void addStr(String* str);
	void addStr(const char* str);
	void addChar(char c);
	void format(const char* str, ...);
	void addInt(int value);
	void addFloat(float value);
	void replace(const char* strToReplace, const char* replaceStr);
	void removeAt(int index, int count);
	int indexOf(char c);
	int indexOf(const char* str);
	String toString();
	void clear();
	char getCharAtIdx(int index) const;
	void insert(char c, int index);
	void free();
};