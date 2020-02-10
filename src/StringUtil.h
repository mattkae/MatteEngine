#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <string>
#include "Matrix4x4f.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"

namespace StringUtil {
	constexpr int DEFAULT_BUFFER_SIZE = 1024;

	inline void substring(char *out, char const *in, size_t len, size_t offset = 0) { 
		memcpy(out, in + offset, len);
		out[len] = '\0';
	}

	inline bool startsWith(const char* inStr, const char* cmpStr) {
		for (size_t index = 0; index < strlen(cmpStr); index++) {
			if (inStr[index] != cmpStr[index]) {
				return false;
			}
		}

		return true;
	}

	inline void trimLeft(char*& ptr) {
		while (ptr[0] == ' ' || ptr[0] == '\t') {
			ptr++;
		}
	}

	inline bool ifEqualWalkToValue(char*& inStr, const char* cmpStr) {
		if (startsWith(inStr, cmpStr)) {
			inStr += strlen(cmpStr);
			trimLeft(inStr);
			return true;
		}
		return false;
	}

	inline void tryWalkToNextCharacter(char*& inStr, char c) {
		while (inStr[0] != c && inStr[0] != '\0') {
			inStr++;
		}
	}

	inline size_t getLengthToEndOfLine(char* ptr) {
		size_t result = 0;
		while (ptr[0] != '\n' && ptr[0] != '\0') {
			result++;
			ptr++;
		}
		return result;
	}

	inline size_t getLengthToChar(char* ptr, char c) {
		size_t result = 0;
		while (ptr[0] != c && ptr[0] != '\0') {
			result++;
			ptr++;
		}
		return result;
	}

	inline size_t getLengthToSpace(char* ptr) {
		size_t result = 0;
		while (ptr[0] != ' ' && ptr[0] != '\t' && ptr[0] != '\n' && ptr[0] != '\0') {
			result++;
			ptr++;
		}
		return result;
	}

	inline bool processLine(FILE* file, char buffer[DEFAULT_BUFFER_SIZE], char*& ptr) {
		if (fgets(buffer, DEFAULT_BUFFER_SIZE * sizeof(char), file) != NULL) {
			size_t bufferSize = strlen(buffer);
			buffer[bufferSize - 1] = '\0';

			ptr = &buffer[0];
			trimLeft(ptr);
			return true;
		}

		return false;
	}

	inline void strToVec3(char* ptr, Vector3f& v) {
		size_t index = 0;
		char value[32];
		while (index < 3 && ptr[0] != '\0' && ptr[0] != '\n') {
			size_t toSpace = getLengthToSpace(ptr);
			substring(value, ptr, toSpace);
			ptr += toSpace + 1;
			switch (index) {
			case 0:
				v.x = std::stof(value);
				break;
			case 1:
				v.y = std::stof(value);
				break;
			case 2:
				v.z = std::stof(value);
				break;
			}
			index++;
		}
	}

	inline void strToVec2(char* ptr, Vector2f& v) {
		size_t index = 0;
		char value[32];
		while (index < 2 && ptr[0] != '\0' && ptr[0] != '\n') {
			size_t toSpace = getLengthToSpace(ptr);
			substring(value, ptr, toSpace);
			ptr += toSpace + 1;
			switch (index) {
			case 0:
				v.x = std::stof(value);
				break;
			case 1:
				v.y = std::stof(value);
				break;
			}
			index++;
		}
	}

	inline void strToVec4(char* ptr, Vector4f& v) {
		size_t index = 0;
		char value[32];
		while (index < 4 && ptr[0] != '\0' && ptr[0] != '\n') {
			size_t toSpace = getLengthToSpace(ptr);
			substring(value, ptr, toSpace);
			ptr += toSpace + 1;
			switch (index) {
			case 0:
				v.x = std::stof(value);
				break;
			case 1:
				v.y = std::stof(value);
				break;
			case 2:
				v.z = std::stof(value);
				break;
			case 3:
				v.w = std::stof(value);
				break;
			}
			index++;
		}
	}

	inline void strToMat4(char* ptr, Matrix4x4f& mat) {
		size_t index = 0;
		char value[32];
		while (index < 16 && ptr[0] != '\0' && ptr[0] != '\n') {
			size_t toSpace = getLengthToSpace(ptr);
			substring(value, ptr, toSpace);
			ptr += toSpace + 1;
			float fvalue = std::stof(value);

			if (index < 4) {
				mat.values[index % 4] = fvalue;
			} else if (index < 8) {
				mat.values[4 + index % 4] = fvalue;
			} else if (index < 12) {
				mat.values[8 + index % 4] = fvalue;
			} else {
				mat.values[12 + index % 4] = fvalue;
			}

			index++;
		}
	}

	inline void strToMyMat4(char* ptr, Matrix4x4f& v) {
		size_t index = 0;
		char value[32];
		while (index < 16 && ptr[0] != '\0' && ptr[0] != '\n') {
			size_t toSpace = getLengthToSpace(ptr);
			substring(value, ptr, toSpace);
			ptr += toSpace + 1;
			float fvalue = std::stof(value);

			v.values[index] = fvalue;
			index++;
		}
	}

	inline void strToBool(char* ptr, bool& value) {
		if (startsWith(ptr, "true")) {
			value = true;
		} else {
			value = false;
		}
	}

	inline void strToInt(char* ptr, int& value) {
		char valueBuffer[64];
		substring(valueBuffer, ptr, getLengthToEndOfLine(ptr));
		value = std::stoi(valueBuffer);
	}

	inline void strToFloat(char* ptr, float& value) {
		char valueBuffer[64];
		substring(valueBuffer, ptr, getLengthToEndOfLine(ptr));
		value = std::stof(valueBuffer);
	}

	inline int getMinLengthToAnyChar(char* ptr, char* arr, size_t numElements, char& minChar) {
		int min = INT_MAX;
		for (size_t index = 0; index < numElements; index++) {
			int lengthToChar = getLengthToChar(ptr, arr[index]);
			if (lengthToChar < min) {
				min = lengthToChar;
				minChar = arr[index];
			}
		}
		return min;
	}
}

#endif