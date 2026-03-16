#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputPollOption) {
    auto result = std::make_shared<InputPollOption>();
    parse(data, "text", &result->text);
    parse(data, "text_parse_mode", &result->textParseMode);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    return result;
}

DECLARE_PARSER_TO_JSON(InputPollOption) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("text_parse_mode", object->textParseMode);
        json.put("text_entities", object->textEntities);
    }
    return json;
}

} // namespace TgBot
