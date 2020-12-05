#include "DirectoryReader.h"
#include "TransformToolConstants.h"
#include "Logger.h"
#ifdef WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif // WIN32
#include <iostream>

int endsWith(const char *str, const char *suffix) {
  size_t str_len = strlen(str);
  size_t suffix_len = strlen(suffix);

  return (str_len >= suffix_len) &&
         (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

List<String> readModelDirectory() {
    List<String> retval;
    StringBuilder sb;

	struct dirent *entry;
    DIR *dp;

    dp = opendir(TransformToolConstants::MODEL_DIRECTORY);
     if (dp == NULL) {
        logger_error("Path does not exist or could not be read: %s", TransformToolConstants::MODEL_DIRECTORY);
        return retval;
    }
    
    while ((entry = readdir(dp))) {
        char* directoryName = entry->d_name;

        if (strcmp(directoryName, ".") == 0 || strcmp(directoryName, "..") == 0) {
            continue;
        }

        sb.clear();
        sb.format("%s/%s", TransformToolConstants::MODEL_DIRECTORY, directoryName);
        String dirName = sb.toString();
        logger_info("Reading model directory: %s", dirName.getValue());

        DIR* modelDp = opendir(dirName.getValue());
        if (modelDp == NULL) {
            logger_error("Path does not exist or could not be read: %s", dirName.getValue());
            continue;
        }

        struct dirent* modelDirectoryEntry;
        StringBuilder modelSb;
        while ((modelDirectoryEntry = readdir(modelDp))) {
            char* filename = modelDirectoryEntry->d_name;

            if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
                continue;
            }

            modelSb.clear();
            modelSb.format("%s/%s", dirName.getValue(), filename);
            String fileInModelDirectory = modelSb.toString();

            if (endsWith(fileInModelDirectory.getValue(), ".obj") == 1 || endsWith(fileInModelDirectory.getValue(), ".dae") == 1) {
                logger_info("Found model file in model directory: %s", fileInModelDirectory.getValue());
                retval.add(fileInModelDirectory);
            } else {
                fileInModelDirectory.free();
            }
        }

        dirName.free();
    }

    sb.free();

    closedir(dp);
    return retval;
}