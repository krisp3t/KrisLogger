#ifdef __ANDROID__
#include <android/log.h>
#endif

#include <iostream>
#include <chrono>
#include <cstdarg>
#include <KrisLogger/Logger.h>

namespace KrisLogger
{
#ifdef NDEBUG
    LogLevel Logger::_minLevel = LogLevel::LOG_INFO;
#else
    LogLevel Logger::_minLevel = LogLevel::LOG_DEBUG;
#endif
    const char *Logger::_tag = "KrisLogger";

    // TODO: make into sink
    const char *color_reset = "\u001b[0m";
    const char *color_info = "\u001b[37m";  // White
    const char *color_debug = "\u001b[36m"; // Cyan
    const char *color_warn = "\u001b[33m";  // Yellow
    const char *color_error = "\u001b[31m"; // Red

    void Logger::SetLogLevel(LogLevel level)
    {
        _minLevel = level;
    }

    void Logger::SetTag(const char* tag)
    {
        _tag = tag;
    }

    void Logger::Log(LogLevel level, const char *file, int line, const char *format, ...)
    {
        if (level < _minLevel)
        {
            return;
        }

        constexpr int MAX_LOG_LENGTH = 1024; // TODO: replace fixed-size buffers

        va_list args;
        va_start(args, format);
        char formatted_message[MAX_LOG_LENGTH];
        vsnprintf(formatted_message, MAX_LOG_LENGTH - 1, format, args);
        va_end(args);
        formatted_message[MAX_LOG_LENGTH - 1] = '\0'; // Ensure null termination

#ifdef __ANDROID__
        android_LogPriority androidPriority;
        switch (level)
        {
        case LogLevel::LOG_INFO:  androidPriority = ANDROID_LOG_INFO;    break;
        case LogLevel::LOG_DEBUG: androidPriority = ANDROID_LOG_DEBUG;   break;
        case LogLevel::LOG_WARN:  androidPriority = ANDROID_LOG_WARN;    break;
        case LogLevel::LOG_ERROR: androidPriority = ANDROID_LOG_ERROR;   break;
        default:                  androidPriority = ANDROID_LOG_UNKNOWN; break;
        }

        __android_log_print(androidPriority, _tag, "%s (%s:%d)", formatted_message, file, line);
#else
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&localTime, &nowTime);
#else
        localtime_r(&nowTime, &localTime);
#endif
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

        char timeBuffer1[60];
        strftime(timeBuffer1, sizeof(timeBuffer1), "%Y-%m-%d %H:%M:%S", &localTime);
        char timeBuffer2[80];
#if defined(_WIN32) || defined(_WIN64)
        sprintf_s(timeBuffer2, sizeof(timeBuffer2), "%s.%03d", timeBuffer1, (int)millis);
#else
        snprintf(timeBuffer2, sizeof(timeBuffer2), "%s.%03d", timeBuffer1, (int)millis);
#endif
        std::cout << "[" << timeBuffer2 << "] ";

        switch (level)
        {
        case LogLevel::LOG_INFO:
            std::cout << color_info << "[INFO] ";
            break;
        case LogLevel::LOG_DEBUG:
            std::cout << color_debug << "[DEBUG] ";
            break;
        case LogLevel::LOG_WARN:
            std::cerr << color_warn << "[WARNING] ";
            break;
        case LogLevel::LOG_ERROR:
            std::cerr << color_error << "[ERROR] ";
            break;
        }

        std::cout << formatted_message << color_reset << std::endl;

        if (level > LogLevel::LOG_INFO)
        {
            std::cout << file << ":" << line << std::endl;
        }
#endif
}

} // namespace KrisLogger