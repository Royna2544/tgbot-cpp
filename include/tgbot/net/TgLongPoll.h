#ifndef TGBOT_TGLONGPOLL_H
#define TGBOT_TGLONGPOLL_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "tgbot/Api.h"
#include "tgbot/export.h"

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
    using timeout_t = Api::optional_default<std::int32_t, 0>;
    using limit_t = Api::bounded_optional_default<std::int32_t, 0, 100, 100>;
    explicit TgLongPoll(Bot* bot, timeout_t timeout, limit_t limit,
                        Update::Types allowedUpdates);

    /**
     * @brief Starts long poll. After new update will come, this method will
     * parse it and send to EventHandler which invokes your listeners. Designed
     * to be executed in a loop.
     */
    void start();

   private:
    Bot* _bot;
    std::int32_t _lastUpdateId = 0;
    limit_t _limit;
    timeout_t _timeout;
    Update::Types _allowedUpdates;
    std::vector<Update::Ptr> _updates;
};

}  // namespace TgBot

#endif  // TGBOT_TGLONGPOLL_H
