#ifndef TGBOT_INPUTCHECKLIST_H
#define TGBOT_INPUTCHECKLIST_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/InputChecklistTask.h"
#include "tgbot/types/MessageEntity.h"

namespace TgBot {

/**
 * @brief Describes a checklist to create.
 *
 * @ingroup types
 */
class InputChecklist {
public:
    using Ptr = std::shared_ptr<InputChecklist>;

    /**
     * @brief Title of the checklist; 1-255 characters after entities parsing
     */
    std::string title;

    /**
     * @brief Optional. Mode for parsing entities in the title. See formatting options for more details.
     */
    std::optional<std::string> parseMode;

    /**
     * @brief Optional. List of special entities that appear in the title, which can be specified instead of parse_mode. Currently, only bold, italic, underline, strikethrough, spoiler, and custom_emoji entities are allowed.
     */
    std::optional<std::vector<MessageEntity::Ptr>> titleEntities;

    /**
     * @brief List of 1-30 tasks in the checklist
     */
    std::vector<InputChecklistTask::Ptr> tasks;

    /**
     * @brief Optional. Pass True if other users can add tasks to the checklist
     */
    std::optional<bool> othersCanAddTasks;

    /**
     * @brief Optional. Pass True if other users can mark tasks as done or not done in the checklist
     */
    std::optional<bool> othersCanMarkTasksAsDone;

};
}

#endif //TGBOT_INPUTCHECKLIST_H
