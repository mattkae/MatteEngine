#pragma once
#include <fstream>
#include "ReadonlyString.h"
#include "Logger.h"

namespace FileHelper {
	inline FILE* openFile(const char* filepath) {
		FILE* file;
		#ifdef __APPLE__
			file = fopen(filepath, "r");
		#else
			fopen_s(&file, filepath, "r");
		#endif
		return file;
	}

	inline void readNext(FILE* file, ReadonlyString& buffer) {
		size_t bytesRead = fread(buffer.data, 1, buffer.bufferSize, file);
		buffer.length = bytesRead;
		buffer.ptr = buffer.data;
	}

	namespace XML {
		inline bool readTag(FILE* file, ReadonlyString& buffer) {
			if (buffer.data == nullptr) {
				buffer.allocate(1024);
			}

			readNext(file, buffer);

			int startIndex = 0;
			while (buffer.ptr[startIndex] != '<') {
				if (buffer.ptr[startIndex] != ' ' && buffer.ptr[startIndex] != '\n') {
					Logger::logWarning("Encountered unknown character when we should be encountering an '<'");
					return false;
				}
				startIndex++;
			}

			buffer.ptr += startIndex;

			int endIndex = startIndex + 1;
			while (buffer.ptr[endIndex] != '>') {
				if (endIndex == buffer.bufferSize) {
					buffer.grow(2 * buffer.bufferSize);
					fseek(file, buffer.bufferSize, SEEK_SET);
					readNext(file, buffer);
				}

				endIndex++;
			}


			buffer.bufferSize = endIndex - startIndex;
			buffer.data[endIndex + 1] = '\0';
		}
	}
}