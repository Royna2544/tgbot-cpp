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
    explicit TgLongPoll(Bot* bot, std::int32_t timeout, std::int32_t limit, std::vector<std::string> allowedUpdates);

    /**
     * @brief Starts long poll. After new update will come, this method will parse it and send to EventHandler which invokes your listeners. Designed to be executed in a loop.
     */
    void start();

private:
    Bot* _bot;
    std::int32_t _lastUpdateId = 0;
    std::int32_t _limit;
    std::int32_t _timeout;
    std::vector<std::string> _allowedUpdates;
    std::vector<Update::Ptr> _updates;
};

}

#endif //TGBOT_TGLONGPOLL_H
