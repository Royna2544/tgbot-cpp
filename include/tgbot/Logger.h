#ifndef TGBOT_LOGGER_H
#define TGBOT_LOGGER_H

#include <memory>
#include <string>

#include "tgbot/export.h"

namespace TgBot {

/**
 * @brief Severity levels for library log messages.
 *
 * @ingroup tools
 */
enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Off,
};

/**
 * @brief Sink interface for the library's diagnostics.
 *
 * Implement this interface and register an instance with setLogger() to route
 * tgbot-cpp's internal log messages into your own logging backend (for example
 * spdlog, glog, or std::clog). The library itself depends on nothing but this
 * interface.
 *
 * @ingroup tools
 */
class TGBOT_API Logger {
   public:
    virtual ~Logger() = default;

    /**
     * @brief Receives a single log record.
     *
     * Implementations must be thread-safe if the bot is used from multiple
     * threads.
     */
    virtual void log(LogLevel level, const std::string& message) = 0;
};

/**
 * @brief Default Logger that writes records at or above a minimum level to
 * stderr. Used unless a custom logger is registered.
 *
 * @ingroup tools
 */
class TGBOT_API DefaultLogger : public Logger {
   public:
    explicit DefaultLogger(LogLevel minLevel = LogLevel::Warning);
    void log(LogLevel level, const std::string& message) override;

   private:
    LogLevel _minLevel;
};

/**
 * @brief Registers the logger used by the library. Pass nullptr to restore the
 * default (stderr) logger. Thread-safe.
 *
 * @ingroup tools
 */
TGBOT_API void setLogger(std::shared_ptr<Logger> logger);

/**
 * @brief Returns the currently registered logger (never null). Thread-safe.
 *
 * @ingroup tools
 */
TGBOT_API std::shared_ptr<Logger> getLogger();

namespace detail {

/**
 * @brief Convenience wrapper used by the library to emit a log record through
 * the registered logger.
 */
TGBOT_API void log(LogLevel level, const std::string& message);

}  // namespace detail

}  // namespace TgBot

#endif  // TGBOT_LOGGER_H
