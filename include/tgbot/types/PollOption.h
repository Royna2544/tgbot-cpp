#ifndef TGBOT_POLLOPTION_H
#define TGBOT_POLLOPTION_H

#include "tgbot/types/MessageEntity.h"

#include <cstdint>
#include <memory>
#include <string>

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
};

}

#endif //TGBOT_POLLOPTION_H
