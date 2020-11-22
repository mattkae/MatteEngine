#ifndef LOGGER_H
#define LOGGER_H

enum LogLevel {
    LogLevel_Debug = 0,
    LogLevel_Info = 1,
    LogLevel_Warn = 2,
    LogLevel_Error = 3
};

struct LoggerOptions {
    LogLevel level = LogLevel_Debug;
    bool logToFile = false;
    const char* filePath = "debug.log";
};

namespace Logger {
    void initialize(LoggerOptions options);
    void setLevel(LogLevel level);
    LogLevel getLevel();
    void log(LogLevel level, const char* format, ...);
    void debug(const char* format, ...);
    void info(const char* format, ...);
    void warning(const char* format, ...);
    void error(const char* format, ...);
    void free();
};
#endif