#ifndef TGBOT_SUGGESTEDPOSTPAID_H
#define TGBOT_SUGGESTEDPOSTPAID_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/StarAmount.h"

namespace TgBot {

class Message;

/**
 * @brief Describes a service message about a successful payment for a suggested post.
 *
 * @ingroup types
 */
class SuggestedPostPaid {
public:
    using Ptr = std::shared_ptr<SuggestedPostPaid>;

    /**
     * @brief Optional. Message containing the suggested post. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<std::shared_ptr<Message>> suggestedPostMessage;

    /**
     * @brief Currency in which the payment was made. Currently, one of "XTR" for Telegram Stars or "TON" for toncoins
     */
    std::string currency;

    /**
     * @brief Optional. The amount of the currency that was received by the channel in nanotoncoins; for payments in toncoins only
     */
    std::optional<std::int64_t> amount;

    /**
     * @brief Optional. The amount of Telegram Stars that was received by the channel; for payments in Telegram Stars only
     */
    std::optional<StarAmount::Ptr> starAmount;

};
}

#endif //TGBOT_SUGGESTEDPOSTPAID_H
