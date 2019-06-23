#include <chrono>
#include <iostream>
#include <sstream>
#include "Logger.h"

namespace Logger {
    std::string printTime()
    {
        time_t t = time(0);
        tm now;
#ifdef WIN32
        localtime_s(&now, &t);
#else
        now = *localtime(&t);
#endif
        std::stringstream ss;
        ss << (now.tm_year + 1900) << '-'
           << (now.tm_mon + 1) << '-'
           << now.tm_mday << ' '
           << now.tm_hour << ':'
           << now.tm_min << ':'
           << now.tm_sec;
        return ss.str();
    }

    void logError(std::string error)
    {
        std::cerr << "[" << printTime() << "] " << "Error: " << error << '\n';
    }

    void logWarning(std::string warning)
    {
        std::cout << "[" << printTime() << "] " << "Warning: " << warning << '\n';
    }

    void logInfo(std::string info)
    {
        std::clog << "[" << printTime() << "] " << "Info: " << info << '\n';
    }
}