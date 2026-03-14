#ifndef TGBOT_CHECKLIST_H
#define TGBOT_CHECKLIST_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/ChecklistTask.h"
#include "tgbot/types/MessageEntity.h"

namespace TgBot {

/**
 * @brief Describes a checklist.
 *
 * @ingroup types
 */
class Checklist {
public:
    using Ptr = std::shared_ptr<Checklist>;

    /**
     * @brief Title of the checklist
     */
    std::string title;

    /**
     * @brief Optional. Special entities that appear in the checklist title
     */
    std::optional<std::vector<MessageEntity::Ptr>> titleEntities;

    /**
     * @brief List of tasks in the checklist
     */
    std::vector<ChecklistTask::Ptr> tasks;

    /**
     * @brief Optional. True, if users other than the creator of the list can add tasks to the list
     */
    std::optional<bool> othersCanAddTasks;

    /**
     * @brief Optional. True, if users other than the creator of the list can mark tasks as done or not done
     */
    std::optional<bool> othersCanMarkTasksAsDone;

};
}

#endif //TGBOT_CHECKLIST_H
