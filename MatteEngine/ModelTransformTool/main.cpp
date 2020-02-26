#include <iostream>
#ifdef WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif // WIN32


int main() {
    struct dirent *entry;
    DIR *dp;

    dp = opendir("assets/models");
     if (dp == NULL) {
        perror("opendir: Path does not exist or could not be read.");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dp)))
    puts(entry->d_name);

    closedir(dp);
    return EXIT_SUCCESS;
}