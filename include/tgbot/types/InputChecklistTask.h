#ifndef TGBOT_INPUTCHECKLISTTASK_H
#define TGBOT_INPUTCHECKLISTTASK_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/MessageEntity.h"

namespace TgBot {

/**
 * @brief Describes a task to add to a checklist.
 *
 * @ingroup types
 */
class InputChecklistTask {
public:
    using Ptr = std::shared_ptr<InputChecklistTask>;

    /**
     * @brief Unique identifier of the task; must be positive and unique among all task identifiers currently present in the checklist
     */
    std::int64_t id;

    /**
     * @brief Text of the task; 1-100 characters after entities parsing
     */
    std::string text;

    /**
     * @brief Optional. Mode for parsing entities in the text. See formatting options for more details.
     */
    std::optional<std::string> parseMode;

    /**
     * @brief Optional. List of special entities that appear in the text, which can be specified instead of parse_mode. Currently, only bold, italic, underline, strikethrough, spoiler, and custom_emoji entities are allowed.
     */
    std::optional<std::vector<MessageEntity::Ptr>> textEntities;

};
}

#endif //TGBOT_INPUTCHECKLISTTASK_H
