#pragma once

#include <vector>
#include <string>

enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
    public:
        static std::vector<LogEntry> messages;
        static void Log(const std::string& message);
        static void Err(const std::string& message);
};
