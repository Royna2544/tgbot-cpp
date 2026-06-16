#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChecklistTasksDone.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChecklistTasksDone> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChecklistTasksDone>();
    result->checklistMessage = parse<Message>(data, "checklist_message");
    result->markedAsDoneTaskIds = parsePrimitiveArray<std::int64_t>(data, "marked_as_done_task_ids");
    result->markedAsNotDoneTaskIds = parsePrimitiveArray<std::int64_t>(data, "marked_as_not_done_task_ids");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChecklistTasksDone> &object) {
    JsonWrapper json;
    if (object) {
        json.put("checklist_message", object->checklistMessage);
        json.put("marked_as_done_task_ids", object->markedAsDoneTaskIds);
        json.put("marked_as_not_done_task_ids", object->markedAsNotDoneTaskIds);
    }
    return json;
}

} // namespace TgBot
