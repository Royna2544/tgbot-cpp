#ifndef TGBOT_CHATBOOSTSOURCEGIVEAWAY_H
#define TGBOT_CHATBOOSTSOURCEGIVEAWAY_H

#include "tgbot/types/ChatBoostSource.h"

#include <cstdint>
#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief The boost was obtained by the creation of a Telegram Premium giveaway.
 *
 * This boosts the chat 4 times for the duration of the corresponding Telegram Premium subscription.
 *
 * @ingroup types
 */
class ChatBoostSourceGiveaway : public ChatBoostSource {

public:
    static constexpr std::string_view SOURCE = "giveaway";

    using Ptr = std::shared_ptr<ChatBoostSourceGiveaway>;

    ChatBoostSourceGiveaway() {
        this->source = SOURCE;
    }

    /**
     * @brief Identifier of a message in the chat with the giveaway; the message could have been deleted already.
     *
     * May be 0 if the message isn't sent yet.
     */
    std::int32_t giveawayMessageId{};

    /**
     * @brief Optional. True, if the giveaway was completed, but there was no user to win the prize
     */
    std::optional<bool> isUnclaimed;
};
}

#endif //TGBOT_CHATBOOSTSOURCEGIVEAWAY_H
