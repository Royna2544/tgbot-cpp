#ifndef TGBOT_SUGGESTEDPOSTAPPROVALFAILED_H
#define TGBOT_SUGGESTEDPOSTAPPROVALFAILED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/SuggestedPostPrice.h"

namespace TgBot {

class Message;

/**
 * @brief Describes a service message about the failed approval of a suggested post. Currently, only caused by insufficient user funds at the time of approval.
 *
 * @ingroup types
 */
class SuggestedPostApprovalFailed {
public:
    using Ptr = std::shared_ptr<SuggestedPostApprovalFailed>;

    /**
     * @brief Optional. Message containing the suggested post whose approval has failed. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<std::shared_ptr<Message>> suggestedPostMessage;

    /**
     * @brief Expected price of the post
     */
    SuggestedPostPrice::Ptr price;

};
}

#endif //TGBOT_SUGGESTEDPOSTAPPROVALFAILED_H
