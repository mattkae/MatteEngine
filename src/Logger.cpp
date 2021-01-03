#include "Logger.h"
#include <chrono>
#include <cstdarg>
#include <cstdio>

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

    void printHeader(const char* levelStr, const char* fileName, int lineNumber) {
        time_t t = time(0);
        tm now;
#ifdef WIN32
        localtime_s(&now, &t);
#else
        now = *localtime(&t);
#endif

        printf("%s:%d [%d-%d-%d %d:%d:%d] %s: ", fileName, lineNumber, (now.tm_year + 1900), (now.tm_mon + 1), now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, levelStr);
        if (gFilePointer != NULL) {
            fprintf(gFilePointer, "[%d-%d-%d %d:%d:%d] %s: ", (now.tm_year + 1900), (now.tm_mon + 1), now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, levelStr);
        }
    }

    void logInternal(const char* file, int lineNumber,LogLevel level, const char* format, va_list args) {
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

	if (gFilePointer != NULL) {
	    va_list fileArgs;
	    va_copy(fileArgs, args);
	    vfprintf(gFilePointer, format, fileArgs);
	    fprintf(gFilePointer, "\n");
	}

        printHeader(levelStr, file, lineNumber);

        vprintf(format, args);
        printf("\n");
    }

    void doLog(const char* file, int lineNumber,LogLevel level, const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(file, lineNumber, level, format, args);
        va_end(args);
    }

    void doDebug(const char* file, int lineNumber,const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(file, lineNumber, LogLevel_Debug, format, args);
        va_end(args);
    }

    void doInfo(const char* file, int lineNumber,const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(file, lineNumber, LogLevel_Info, format, args);
        va_end(args);
    }

    void doWarning(const char* file, int lineNumber,const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(file, lineNumber, LogLevel_Warn, format, args);
        va_end(args);
    }

    void doError(const char* file, int lineNumber,const char* format, ...) {
        va_list args;
        va_start(args, format);
        logInternal(file, lineNumber, LogLevel_Error, format, args);
        va_end(args);
    }

    void free() {
        if (gFilePointer) {
            fclose(gFilePointer);
	    gFilePointer = NULL;
        }
    }
}
