#include "DirectoryReader.h"
#include "Include.h"
#include "TransformToolConstants.h"
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


List<String> readModelDirectory() {
    List<String> retval;

	struct dirent *entry;
    DIR *dp;

    dp = opendir(TransformToolConstants::MODEL_DIRECTORY);
     if (dp == NULL) {
        Logger::logError("Could not open the path to the model directory.");
        return retval;
    }
    
    while ((entry = readdir(dp))) {
        String directoryName = entry->d_name;
        if (directoryName == "." || directoryName == "..") {
            continue;
        }

        String modelDirectory = TransformToolConstants::MODEL_DIRECTORY;
        modelDirectory.append('/'); 
        modelDirectory.append(directoryName);
        //Logger::logInfo("Reading model directory: " + modelDirectory.value);

        DIR* modelDp = opendir(modelDirectory.value);
        if (modelDp == NULL) {
            //Logger::logError("Path does not exist or could not be read: " + modelDirectory);
            continue;
        }

        struct dirent* modelDirectoryEntry;
        while ((modelDirectoryEntry = readdir(modelDp))) {
            String filename = modelDirectoryEntry->d_name;

            if (filename == "." || filename == "..") {
                continue;
            }

            String fileInModelDirectory = modelDirectory;
            fileInModelDirectory.append('/');
            fileInModelDirectory.append(filename);

            if (fileInModelDirectory.endsWith(".obj") || fileInModelDirectory.endsWith(".dae")) {
                //Logger::logInfo("Found model file in model directory: " + fileInModelDirectory);
                retval.add(&fileInModelDirectory);
            }
        }

        directoryName.deallocate();
        modelDirectory.deallocate();
    }

    closedir(dp);
    return retval;
}