#ifndef TGBOT_CHECKLISTTASK_H
#define TGBOT_CHECKLISTTASK_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Chat.h"
#include "tgbot/types/MessageEntity.h"
#include "tgbot/types/User.h"

namespace TgBot {

/**
 * @brief Describes a task in a checklist.
 *
 * @ingroup types
 */
class ChecklistTask {
public:
    using Ptr = std::shared_ptr<ChecklistTask>;

    /**
     * @brief Unique identifier of the task
     */
    std::int64_t id;

    /**
     * @brief Text of the task
     */
    std::string text;

    /**
     * @brief Optional. Special entities that appear in the task text
     */
    std::optional<std::vector<MessageEntity::Ptr>> textEntities;

    /**
     * @brief Optional. User that completed the task; omitted if the task wasn't completed by a user
     */
    std::optional<User::Ptr> completedByUser;

    /**
     * @brief Optional. Chat that completed the task; omitted if the task wasn't completed by a chat
     */
    std::optional<Chat::Ptr> completedByChat;

    /**
     * @brief Optional. Point in time (Unix timestamp) when the task was completed; 0 if the task wasn't completed
     */
    std::optional<std::int64_t> completionDate;

};
}

#endif //TGBOT_CHECKLISTTASK_H
