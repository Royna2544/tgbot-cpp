#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Checklist) {
    auto result = std::make_shared<Checklist>();
    parse(data, "title", &result->title);
    result->titleEntities = parseArray<MessageEntity>(data, "title_entities");
    result->tasks = parseRequiredArray<ChecklistTask>(data, "tasks");
    parse(data, "others_can_add_tasks", &result->othersCanAddTasks);
    parse(data, "others_can_mark_tasks_as_done", &result->othersCanMarkTasksAsDone);
    return result;
}

DECLARE_PARSER_TO_JSON(Checklist) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("title_entities", object->titleEntities);
        json.put("tasks", object->tasks);
        json.put("others_can_add_tasks", object->othersCanAddTasks);
        json.put("others_can_mark_tasks_as_done", object->othersCanMarkTasksAsDone);
    }
    return json;
}

} // namespace TgBot
