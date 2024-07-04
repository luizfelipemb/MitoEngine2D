#include "Logger.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include "../imgui/imgui.h"

std::vector<LogEntry> Logger::Messages;

std::string CurrentDateTimeToString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%H:%M:%S", std::localtime(&now));
    output.resize(std::strlen(output.c_str())); // Resize to actual length
    return output;
}

void Logger::Log(const std::string& message)
{
    LogEntry logEntry;
    logEntry.Type = LOG_INFO;
    logEntry.Message = CurrentDateTimeToString() + ": " + message;
    std::cout << "\x1B[32m" << logEntry.Message << "\033[0m" << std::endl;
    Messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
    LogEntry logEntry;
    logEntry.Type = LOG_ERROR;
    logEntry.Message = CurrentDateTimeToString() + ": " + message;
    Messages.push_back(logEntry);
    std::cerr << "\x1B[91m"<< logEntry.Message << "\033[0m" << std::endl;
}
