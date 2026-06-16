#ifndef TGBOT_POLLOPTION_H
#define TGBOT_POLLOPTION_H

#include "tgbot/types/MessageEntity.h"
#include "tgbot/types/Chat.h"
#include "tgbot/types/PollMedia.h"
#include "tgbot/types/User.h"

#include <cstdint>
#include <memory>
#include <string>
#include <optional>

namespace TgBot {

class PollOption {

public:
    using Ptr = std::shared_ptr<PollOption>;

    /**
     * @brief Option text, 1-100 characters.
     */
    std::string text;

    /**
     * @brief Number of users that voted for this option.
     */
    std::int64_t voterCount;

    /**
     * @brief Optional. Special entities that appear in the option text. Currently, only custom emoji entities are allowed in poll option texts
     */
    std::optional<std::vector<MessageEntity::Ptr>> textEntities;
    /**
     * @brief Unique identifier of the option, persistent on option addition and deletion
     */
    std::string persistentId;

    /**
     * @brief Optional. Media added to the poll option
     */
    std::optional<PollMedia::Ptr> media;

    /**
     * @brief Optional. User who added the option; omitted if the option wasn't added by a user after poll creation
     */
    std::optional<User::Ptr> addedByUser;

    /**
     * @brief Optional. Chat that added the option; omitted if the option wasn't added by a chat after poll creation
     */
    std::optional<Chat::Ptr> addedByChat;

    /**
     * @brief Optional. Point in time (Unix timestamp) when the option was added; omitted if the option existed in the original poll
     */
    std::optional<std::int32_t> additionDate;

};

}

#endif //TGBOT_POLLOPTION_H
