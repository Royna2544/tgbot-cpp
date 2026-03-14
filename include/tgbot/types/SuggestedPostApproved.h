#ifndef TGBOT_SUGGESTEDPOSTAPPROVED_H
#define TGBOT_SUGGESTEDPOSTAPPROVED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/SuggestedPostPrice.h"

namespace TgBot {

class Message;

/**
 * @brief Describes a service message about the approval of a suggested post.
 *
 * @ingroup types
 */
class SuggestedPostApproved {
public:
    using Ptr = std::shared_ptr<SuggestedPostApproved>;

    /**
     * @brief Optional. Message containing the suggested post. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<std::shared_ptr<Message>> suggestedPostMessage;

    /**
     * @brief Optional. Amount paid for the post
     */
    std::optional<SuggestedPostPrice::Ptr> price;

    /**
     * @brief Date when the post will be published
     */
    std::int64_t sendDate;

};
}

#endif //TGBOT_SUGGESTEDPOSTAPPROVED_H
