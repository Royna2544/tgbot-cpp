#ifndef TGBOT_INPUTPOLLOPTION_H
#define TGBOT_INPUTPOLLOPTION_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/MessageEntity.h"

namespace TgBot {

/**
 * @brief This object contains information about one answer option in a poll to be sent.
 *
 * @ingroup types
 */
class InputPollOption {
public:
    using Ptr = std::shared_ptr<InputPollOption>;

    /**
     * @brief Option text, 1-100 characters
     */
    std::string text;

    /**
     * @brief Optional. Mode for parsing entities in the text. See formatting options for more details. Currently, only custom emoji entities are allowed
     */
    std::optional<std::string> textParseMode;

    /**
     * @brief Optional. A JSON-serialized list of special entities that appear in the poll option text. It can be specified instead of text_parse_mode
     */
    std::optional<std::vector<MessageEntity::Ptr>> textEntities;

};
}

#endif //TGBOT_INPUTPOLLOPTION_H
