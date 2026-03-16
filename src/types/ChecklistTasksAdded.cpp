#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChecklistTasksAdded) {
    auto result = std::make_shared<ChecklistTasksAdded>();
    result->checklistMessage = parse<Message>(data, "checklist_message");
    result->tasks = parseRequiredArray<ChecklistTask>(data, "tasks");
    return result;
}

DECLARE_PARSER_TO_JSON(ChecklistTasksAdded) {
    JsonWrapper json;
    if (object) {
        json.put("checklist_message", object->checklistMessage);
        json.put("tasks", object->tasks);
    }
    return json;
}

} // namespace TgBot
