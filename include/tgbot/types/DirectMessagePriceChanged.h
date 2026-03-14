#ifndef TGBOT_DIRECTMESSAGEPRICECHANGED_H
#define TGBOT_DIRECTMESSAGEPRICECHANGED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes a service message about a change in the price of direct messages sent to a channel chat.
 *
 * @ingroup types
 */
class DirectMessagePriceChanged {
public:
    using Ptr = std::shared_ptr<DirectMessagePriceChanged>;

    /**
     * @brief True, if direct messages are enabled for the channel chat; false otherwise
     */
    bool areDirectMessagesEnabled;

    /**
     * @brief Optional. The new number of Telegram Stars that must be paid by users for each direct message sent to the channel. Does not apply to users who have been exempted by administrators. Defaults to 0.
     */
    std::optional<std::int64_t> directMessageStarCount;

};
}

#endif //TGBOT_DIRECTMESSAGEPRICECHANGED_H
