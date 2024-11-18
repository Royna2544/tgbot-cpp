#include "tgbot/net/TgLongPoll.h"

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "tgbot/ApiImpl.h"
#include "tgbot/Bot.h"
#include "tgbot/EventHandler.h"

namespace TgBot {

TgLongPoll::TgLongPoll(Bot* bot, std::int32_t timeout, std::int32_t limit,
                       std::vector<std::string> allowUpdates)
    : _bot(bot),
      _limit(limit),
      _timeout(timeout),
      _allowedUpdates(std::move(allowUpdates)) {
    _bot->_httpClient->_timeout = _timeout + 5;
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
