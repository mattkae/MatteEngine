#pragma once
#include <fstream>

namespace FileHelper {
	inline FILE* openFile(const char* filepath, const char* mode = "r") {
		FILE* file;
		#ifdef WIN32
		fopen_s(&file, filepath, mode);
		#else
		file = fopen(filepath, mode);
		#endif
		return file;
	}
}
