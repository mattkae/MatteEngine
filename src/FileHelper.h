#pragma once
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
}