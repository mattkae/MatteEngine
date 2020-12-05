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
    void doLog(const char* file, int lineNumber, LogLevel level, const char* format, ...);
    void doDebug(const char* file, int lineNumber, const char* format, ...);
    void doInfo(const char* file, int lineNumber, const char* format, ...);
    void doWarning(const char* file, int lineNumber, const char* format, ...);
    void doError(const char* file, int lineNumber, const char* format, ...);
    void free();
};

#if WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#elif
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define logger_log(level, format, ...) Logger::doLog(__FILENAME__, __LINE__, level, format, __VA_ARGS__)
#define logger_debug(format, ...) Logger::doDebug(__FILENAME__, __LINE__, format, __VA_ARGS__)
#define logger_info(format, ...) Logger::doInfo(__FILENAME__, __LINE__, format, __VA_ARGS__)
#define logger_warning(format, ...) Logger::doWarning(__FILENAME__, __LINE__, format, __VA_ARGS__)
#define logger_error(format, ...) Logger::doError(__FILENAME__, __LINE__, format, __VA_ARGS__)

#endif