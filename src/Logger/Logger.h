#pragma once

#include <vector>
#include <string>

enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry {
    LogType Type;
    std::string Message;
};

class Logger {
    public:
        static std::vector<LogEntry> Messages;
        static void Log(const std::string& message);
        static void Err(const std::string& message);
};
