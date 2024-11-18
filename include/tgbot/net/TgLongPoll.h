#ifndef TGBOT_TGLONGPOLL_H
#define TGBOT_TGLONGPOLL_H

#include "tgbot/Api.h"
#include "tgbot/export.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace TgBot {

class Bot;
class EventHandler;

/**
 * @brief This class handles long polling and updates parsing.
 *
 * @ingroup net
 */
class TGBOT_API TgLongPoll {

public:
    TgLongPoll(const Bot& bot, std::int32_t limit = 100, std::int32_t timeout = 10, std::vector<std::string> allowedUpdates = {});

    /**
     * @brief Starts long poll. After new update will come, this method will parse it and send to EventHandler which invokes your listeners. Designed to be executed in a loop.
     */
    void start();

private:
    const Api* _api;
    const EventHandler* _eventHandler;
    std::int32_t _lastUpdateId = 0;
    std::int32_t _limit;
    std::int32_t _timeout;
    std::vector<std::string> _allowedUpdates;

    std::vector<Update::Ptr> _updates;
};

}

#endif //TGBOT_TGLONGPOLL_H
