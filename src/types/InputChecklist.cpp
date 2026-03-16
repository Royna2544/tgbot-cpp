#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputChecklist) {
    auto result = std::make_shared<InputChecklist>();
    parse(data, "title", &result->title);
    parse(data, "parse_mode", &result->parseMode);
    result->titleEntities = parseArray<MessageEntity>(data, "title_entities");
    result->tasks = parseRequiredArray<InputChecklistTask>(data, "tasks");
    parse(data, "others_can_add_tasks", &result->othersCanAddTasks);
    parse(data, "others_can_mark_tasks_as_done", &result->othersCanMarkTasksAsDone);
    return result;
}

DECLARE_PARSER_TO_JSON(InputChecklist) {
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
