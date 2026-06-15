#include "tgbot/Logger.h"

#include <iostream>
#include <mutex>
#include <string_view>
#include <utility>

namespace TgBot {

namespace {

std::mutex& loggerMutex() {
    static std::mutex mutex;
    return mutex;
}

std::shared_ptr<Logger>& loggerStorage() {
    static std::shared_ptr<Logger> logger = std::make_shared<DefaultLogger>();
    return logger;
}

constexpr std::string_view levelName(LogLevel level) {
    switch (level) {
        case LogLevel::Trace:
            return "TRACE";
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARN";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Off:
            return "OFF";
    }
    return "?";
}

}  // namespace

DefaultLogger::DefaultLogger(LogLevel minLevel) : _minLevel(minLevel) {}

void DefaultLogger::log(LogLevel level, const std::string& message) {
    if (_minLevel == LogLevel::Off || level < _minLevel) {
        return;
    }
    std::clog << "[tgbot-cpp] [" << levelName(level) << "] " << message << '\n';
}

void setLogger(std::shared_ptr<Logger> logger) {
    std::lock_guard<std::mutex> lock(loggerMutex());
    loggerStorage() =
        logger ? std::move(logger) : std::make_shared<DefaultLogger>();
}

std::shared_ptr<Logger> getLogger() {
    std::lock_guard<std::mutex> lock(loggerMutex());
    return loggerStorage();
}

namespace detail {

void log(LogLevel level, const std::string& message) {
    getLogger()->log(level, message);
}

}  // namespace detail

}  // namespace TgBot
