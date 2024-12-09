#include "tgbot/net/TgLongPoll.h"

#include <chrono>
#include <memory>
#include <vector>

#include "tgbot/Api.h"
#include "tgbot/Bot.h"
#include "tgbot/EventHandler.h"
#include "tgbot/types/Update.h"

namespace TgBot {

TgLongPoll::TgLongPoll(Bot* bot, timeout_t timeout, limit_t limit,
                       Update::Types allowedUpdates)
    : _bot(bot),
      _limit(limit),
      _timeout(timeout),
      _allowedUpdates(allowedUpdates) {

    // If we ask the Telegram bot server to timeout at [timeout]
    // HTTP Clients must timeout after this time
    // Hence define an offset, if the current value is fine, use as is
    constexpr static std::chrono::seconds offset{5};
    _bot->_httpClient->timeout(std::max(_bot->_httpClient->timeout(), std::chrono::seconds(*timeout) + offset));
}

void TgLongPoll::start() {
    // handle updates
    for (Update::Ptr& item : _updates) {
        if (item->updateId >= _lastUpdateId) {
            _lastUpdateId = item->updateId + 1;
        }
        _bot->_eventHandler->handleUpdate(item);
    }

    // confirm handled updates
    _updates = _bot->_api->getUpdates(_lastUpdateId, _limit, _timeout,
                                      _allowedUpdates);
}

}  // namespace TgBot
