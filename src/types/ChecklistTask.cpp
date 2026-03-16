#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChecklistTask) {
    auto result = std::make_shared<ChecklistTask>();
    parse(data, "id", &result->id);
    parse(data, "text", &result->text);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    result->completedByUser = parse<User>(data, "completed_by_user");
    result->completedByChat = parse<Chat>(data, "completed_by_chat");
    parse(data, "completion_date", &result->completionDate);
    return result;
}

DECLARE_PARSER_TO_JSON(ChecklistTask) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("text", object->text);
        json.put("text_entities", object->textEntities);
        json.put("completed_by_user", object->completedByUser);
        json.put("completed_by_chat", object->completedByChat);
        json.put("completion_date", object->completionDate);
    }
    return json;
}

} // namespace TgBot
