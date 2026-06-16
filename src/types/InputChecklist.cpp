#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputChecklist.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputChecklist> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputChecklist>();
    parse(data, "title", &result->title);
    parse(data, "parse_mode", &result->parseMode);
    result->titleEntities = parseArray<MessageEntity>(data, "title_entities");
    result->tasks = parseRequiredArray<InputChecklistTask>(data, "tasks");
    parse(data, "others_can_add_tasks", &result->othersCanAddTasks);
    parse(data, "others_can_mark_tasks_as_done", &result->othersCanMarkTasksAsDone);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputChecklist> &object) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("parse_mode", object->parseMode);
        json.put("title_entities", object->titleEntities);
        json.put("tasks", object->tasks);
        json.put("others_can_add_tasks", object->othersCanAddTasks);
        json.put("others_can_mark_tasks_as_done", object->othersCanMarkTasksAsDone);
    }
    return json;
}

} // namespace TgBot
