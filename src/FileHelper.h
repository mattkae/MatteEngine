#pragma once
#include "ReadonlyString.h"
#include <fstream>

namespace FileHelper {
	inline FILE* openFile(const char* filepath, const char* mode = "r") {
		FILE* file;
		#ifdef __APPLE__
			file = fopen(filepath, mode);
		#else
			fopen_s(&file, filepath, mode);
		#endif
		return file;
	}

	inline void readNext(FILE* file, ReadonlyString& buffer) {
		size_t bytesRead = fread(buffer.data, 1, buffer.bufferSize, file);
		buffer.length = bytesRead;
		buffer.ptr = buffer.data;
	}
}