#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChecklistTasksDone) {
    auto result = std::make_shared<ChecklistTasksDone>();
    result->checklistMessage = parse<Message>(data, "checklist_message");
    result->markedAsDoneTaskIds = parsePrimitiveArray<std::int64_t>(data, "marked_as_done_task_ids");
    result->markedAsNotDoneTaskIds = parsePrimitiveArray<std::int64_t>(data, "marked_as_not_done_task_ids");
    return result;
}

DECLARE_PARSER_TO_JSON(ChecklistTasksDone) {
    JsonWrapper json;
    if (object) {
        json.put("checklist_message", object->checklistMessage);
        json.put("marked_as_done_task_ids", object->markedAsDoneTaskIds);
        json.put("marked_as_not_done_task_ids", object->markedAsNotDoneTaskIds);
    }
    return json;
}

} // namespace TgBot
