#pragma once
#include <string>
#include <cstdio>
#include <cstdarg>

#ifdef DEBUG_OUTPUT
#define LOGD(fmt, ...) logger::debug(LOG_TAG, fmt, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...) logger::emptyFunc()
#endif

#define LOGI(fmt, ...) logger::info(LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) logger::warn(LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) logger::error(LOG_TAG, fmt, ##__VA_ARGS__)

#include <android/log.h>

namespace logger {

    enum class LogLevel {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
    };

    inline void emptyFunc() {}

    void info(const char *tag, const char *fmt, ...);

    void warn(const char *tag, const char *fmt, ...);

    void error(const char *tag, const char *fmt, ...);

    void debug(const char *tag, const char *fmt, ...);

    void log(LogLevel logLevel, const char *tag, const char *msg);

    inline pid_t currentPid = 0;

    inline void info(const char *tag, const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        log(LogLevel::INFO, tag, buf);
    }

    inline void warn(const char *tag, const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        log(LogLevel::WARN, tag, buf);
    }

    inline void error(const char *tag, const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        log(LogLevel::ERROR, tag, buf);
    }

    inline void debug(const char *tag, const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        log(LogLevel::DEBUG, tag, buf);
    }

    inline void log(LogLevel logLevel, const char *tag, const char *msg) {
        switch (logLevel) {
            case LogLevel::DEBUG:
                __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", msg);
                break;
            case LogLevel::INFO:
                __android_log_print(ANDROID_LOG_INFO, tag, "%s", msg);
                break;
            case LogLevel::WARN:
                __android_log_print(ANDROID_LOG_WARN, tag, "%s", msg);
                break;
            case LogLevel::ERROR:
                __android_log_print(ANDROID_LOG_ERROR, tag, "%s", msg);
                break;
        }
        return;
    }
}
