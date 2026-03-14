#ifndef TGBOT_SUGGESTEDPOSTREFUNDED_H
#define TGBOT_SUGGESTEDPOSTREFUNDED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

class Message;

/**
 * @brief Describes a service message about a payment refund for a suggested post.
 *
 * @ingroup types
 */
class SuggestedPostRefunded {
public:
    using Ptr = std::shared_ptr<SuggestedPostRefunded>;

    /**
     * @brief Optional. Message containing the suggested post. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<std::shared_ptr<Message>> suggestedPostMessage;

    /**
     * @brief Reason for the refund. Currently, one of "post_deleted" if the post was deleted within 24 hours of being posted or removed from scheduled messages without being posted, or "payment_refunded" if the payer refunded their payment.
     */
    std::string reason;

};
}

#endif //TGBOT_SUGGESTEDPOSTREFUNDED_H
