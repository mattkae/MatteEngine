#include "DirectoryReader.h"
#include "TransformToolConstants.h"
#include "Logger.h"
#include "Logger.cpp"
#ifdef WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif // WIN32
#include <iostream>

inline bool endsWith(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}


std::vector<std::string> readModelDirectory() {
    std::vector<std::string> retval;

	struct dirent *entry;
    DIR *dp;

    dp = opendir(TransformToolConstants::MODEL_DIRECTORY.c_str());
     if (dp == NULL) {
        Logger::logError("Path does not exist or could not be read: "  + TransformToolConstants::MODEL_DIRECTORY);
        return retval;
    }
    
    while ((entry = readdir(dp))) {
        std::string directoryName = entry->d_name;

        if (directoryName == "." || directoryName == "..") {
            continue;
        }

        std::string modelDirectory = TransformToolConstants::MODEL_DIRECTORY + '/' + directoryName;
        Logger::logInfo("Reading model directory: " + modelDirectory);

        DIR* modelDp = opendir(modelDirectory.c_str());
        if (modelDp == NULL) {
            Logger::logError("Path does not exist or could not be read: " + modelDirectory);
            continue;
        }

        struct dirent* modelDirectoryEntry;
        while ((modelDirectoryEntry = readdir(modelDp))) {
            std::string filename = modelDirectoryEntry->d_name;

            if (filename == "." || filename == "..") {
                continue;
            }

            std::string fileInModelDirectory = modelDirectory + '/' + filename;

            if (endsWith(fileInModelDirectory, ".obj") || endsWith(fileInModelDirectory, ".dae")) {
                Logger::logInfo("Found model file in model directory: " + fileInModelDirectory);
                retval.push_back(fileInModelDirectory);
            }
        }
    }

    closedir(dp);
    return retval;
}