#pragma once
#include <string>

inline std::string getEnclosingFolder(std::string path) {
	size_t lastSlash = path.find_last_of('/');
	if (lastSlash == std::string::npos) {
		return path;
	}

	return path.substr(0, lastSlash);
}
