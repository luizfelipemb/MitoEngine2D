#pragma once

#include <vector>
#include <string>

enum LogType
{
    LOG_INFO,
    LOG_WARNING,
    LOG_LUA,
    LOG_ERROR
};

struct LogEntry
{
    LogType Type;
    std::string Message;
};

class Logger
{
    public:
        static std::vector<LogEntry> Messages;
        static void Log(const std::string& message);
        static void Lua(const std::string& message);
        static void Err(const std::string& message);
        static void Wrn(const std::string& message);
};
