#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputChecklistTask) {
    auto result = std::make_shared<InputChecklistTask>();
    parse(data, "id", &result->id);
    parse(data, "text", &result->text);
    parse(data, "parse_mode", &result->parseMode);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    return result;
}

DECLARE_PARSER_TO_JSON(InputChecklistTask) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("text", object->text);
        json.put("parse_mode", object->parseMode);
        json.put("text_entities", object->textEntities);
    }
    return json;
}

} // namespace TgBot
