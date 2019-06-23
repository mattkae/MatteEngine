#ifndef LOGGER_H
#include <sstream>

namespace Logger {
    void logError(std::string error);
    void logWarning(std::string warning);
    void logInfo(std::string info);
};
#endif