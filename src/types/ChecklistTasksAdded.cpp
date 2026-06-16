#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChecklistTasksAdded.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChecklistTasksAdded> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChecklistTasksAdded>();
    result->checklistMessage = parse<Message>(data, "checklist_message");
    result->tasks = parseRequiredArray<ChecklistTask>(data, "tasks");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChecklistTasksAdded> &object) {
    JsonWrapper json;
    if (object) {
        json.put("checklist_message", object->checklistMessage);
        json.put("tasks", object->tasks);
    }
    return json;
}

} // namespace TgBot
