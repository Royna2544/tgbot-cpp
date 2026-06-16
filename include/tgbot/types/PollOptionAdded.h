#ifndef TGBOT_POLLOPTIONADDED_H
#define TGBOT_POLLOPTIONADDED_H

#include "tgbot/types/MaybeInaccessibleMessage.h"
#include "tgbot/types/MessageEntity.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace TgBot {

/**
 * @brief Describes a service message about an option added to a poll.
 *
 * @ingroup types
 */
class PollOptionAdded {
public:
    using Ptr = std::shared_ptr<PollOptionAdded>;

    /**
     * @brief Optional. Message containing the poll to which the option was added, if known. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<MaybeInaccessibleMessage> pollMessage;

    /**
     * @brief Unique identifier of the added option
     */
    std::string optionPersistentId;

    /**
     * @brief Option text
     */
    std::string optionText;

    /**
     * @brief Optional. Special entities that appear in the option_text
     */
    std::optional<std::vector<MessageEntity::Ptr>> optionTextEntities;

};
}

#endif //TGBOT_POLLOPTIONADDED_H
