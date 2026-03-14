#ifndef TGBOT_CHECKLISTTASKSADDED_H
#define TGBOT_CHECKLISTTASKSADDED_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Checklist.h"
#include "tgbot/types/ChecklistTask.h"

namespace TgBot {

class Message;

/**
 * @brief Describes a service message about tasks added to a checklist.
 *
 * @ingroup types
 */
class ChecklistTasksAdded {
public:
    using Ptr = std::shared_ptr<ChecklistTasksAdded>;

    /**
     * @brief Optional. Message containing the checklist to which the tasks were added. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<std::shared_ptr<Message>> checklistMessage;

    /**
     * @brief List of tasks added to the checklist
     */
    std::vector<ChecklistTask::Ptr> tasks;

};
}

#endif //TGBOT_CHECKLISTTASKSADDED_H
