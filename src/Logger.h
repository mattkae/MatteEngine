#pragma once
#include <iostream>
#include <chrono>
#include <sstream>

class Logger {
private:
    Logger() {}

    void mLogError(std::string error) {
        std::cerr << "[" << printTime() << "] " <<  "Error: " << error << '\n';
    }

    void mLogWarning(std::string warning) {
        std::cout << "[" << printTime() << "] " << "Warning: " << warning << '\n';
    }

    void mLogInfo(std::string info) {
        std::clog << "[" << printTime() << "] " << "Info: " << info << '\n';
    }

    std::string printTime() {
        time_t t = time(0);
        tm* now;
        #ifdef WIN_32
        localtime_s(now, &t);
        #else
        now = localtime(&t);
        #endif
        std::stringstream ss;
        ss << (now->tm_year + 1900) << '-' 
            << (now->tm_mon + 1) << '-'
			<< now->tm_mday << ' '
            << now->tm_hour <<  ':'
            << now->tm_min << ':'
            << now->tm_sec;
        return ss.str();
    }

    static Logger mLogger;
public:
    static Logger& getInstance()
    {
        Logger logger;
        return logger;
    }

    static void logError(std::string error) {
        Logger::getInstance().mLogError(error);
    }

    static void logWarning(std::string warning) {
        Logger::getInstance().mLogWarning(warning);
    }

    static void logInfo(std::string info) {
        Logger::getInstance().mLogInfo(info);
    }
};