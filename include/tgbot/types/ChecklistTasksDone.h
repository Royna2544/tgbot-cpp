#ifndef TGBOT_CHECKLISTTASKSDONE_H
#define TGBOT_CHECKLISTTASKSDONE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

class Message;

/**
 * @brief Describes a service message about checklist tasks marked as done or not done.
 *
 * @ingroup types
 */
class ChecklistTasksDone {
public:
    using Ptr = std::shared_ptr<ChecklistTasksDone>;

    /**
     * @brief Optional. Message containing the checklist whose tasks were marked as done or not done. Note that the Message object in this field will not contain the reply_to_message field even if it itself is a reply.
     */
    std::optional<std::shared_ptr<Message>> checklistMessage;

    /**
     * @brief Optional. Identifiers of the tasks that were marked as done
     */
    std::optional<std::vector<std::int64_t>> markedAsDoneTaskIds;

    /**
     * @brief Optional. Identifiers of the tasks that were marked as not done
     */
    std::optional<std::vector<std::int64_t>> markedAsNotDoneTaskIds;

};
}

#endif //TGBOT_CHECKLISTTASKSDONE_H
