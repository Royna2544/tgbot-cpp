#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(TextQuote) {
    auto result = std::make_shared<TextQuote>();
    parse(data, "text", &result->text);
    result->entities = parseArray<MessageEntity>(data, "entities");
    parse(data, "position", &result->position);
    parse(data, "is_manual", &result->isManual);
    return result;
}

DECLARE_PARSER_TO_JSON(TextQuote) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("entities", object->entities);
        json.put("position", object->position);
        json.put("is_manual", object->isManual);
    }
    return json;
}

} // namespace TgBot
