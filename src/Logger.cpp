#include "Logger.h"
#include <chrono>
#include <cstdarg>

namespace Logger {
    LogLevel gLogLevel = LogLevel_Debug;
    FILE* gFilePointer = NULL;

    void initialize(LoggerOptions options) {
        setLevel(options.level);
        if (options.logToFile) {
#ifdef WIN32
            fopen_s(&gFilePointer, options.filePath, "a");
#else
            gFilePointer = fopen(options.filePath, "a");
#endif
        }
    }

    void setLevel(LogLevel level) {
        gLogLevel = level;
    }

    LogLevel getLevel() {
        return gLogLevel;
    }

    void printHeader(const char* levelStr) {
        time_t t = time(0);
        tm now;
#ifdef WIN32
        localtime_s(&now, &t);
#else
        now = *localtime(&t);
#endif

        printf("[%d-%d-%d %d:%d:%d] %s: ", (now.tm_year + 1900), (now.tm_mon + 1), now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, levelStr);
        if (gFilePointer != NULL) {
            fprintf(gFilePointer, "[%d-%d-%d %d:%d:%d] %s: ", (now.tm_year + 1900), (now.tm_mon + 1), now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, levelStr);
        }
    }

    void logInternal(LogLevel level, const char* format, va_list args) {
        if (level < gLogLevel) {
            return;
        }

        const char* levelStr;
        switch (level) {
        case LogLevel_Debug:
            levelStr = "Debug";
            break;
        case LogLevel_Info:
            levelStr = "Info";
            break;
        case LogLevel_Warn:
            levelStr = "Warning";
            break;
        case LogLevel_Error:
            levelStr = "Error";
            break;
        default:
            levelStr = "Unknown";
            break;
        }

        printHeader(levelStr);

        vprintf(format, args);
        printf("\n");

        if (gFilePointer != NULL) {
            vfprintf(gFilePointer, format, args);
            fprintf(gFilePointer, "\n");
        }
    }

    void log(LogLevel level, const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(level, format, args);
        va_end(args);
    }

    void debug(const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(LogLevel_Debug, format, args);
        va_end(args);
    }

    void info(const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(LogLevel_Info, format, args);
        va_end(args);
    }

    void warning(const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(LogLevel_Warn, format, args);
        va_end(args);
    }

    void error(const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(LogLevel_Error, format, args);
        va_end(args);
    }

    void free() {
        if (gFilePointer) {
            fclose(gFilePointer);
        }
    }
}