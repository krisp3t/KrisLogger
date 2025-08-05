#pragma once

namespace KrisLogger
{

#define LOGI(...) KrisLogger::Logger::Log(KrisLogger::LogLevel::LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOGD(...) KrisLogger::Logger::Log(KrisLogger::LogLevel::LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOGW(...) KrisLogger::Logger::Log(KrisLogger::LogLevel::LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOGE(...) KrisLogger::Logger::Log(KrisLogger::LogLevel::LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

    enum class LogLevel
    {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR
    };

    class Logger
    {
    public:
        static void Log(LogLevel level, const char *file, int line, const char *format, ...);
        static void SetLogLevel(LogLevel level);
        static void SetTag(const char *tag);
    private:
        static LogLevel _minLevel;
        static const char* _tag;
    };

} // namespace KrisLogger