#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChosenInlineResult) {
    auto result = std::make_shared<ChosenInlineResult>();
    parse(data, "result_id", &result->resultId);
    result->from = parseRequired<User>(data, "from");
    result->location = parse<Location>(data, "location");
    parse(data, "inline_message_id", &result->inlineMessageId);
    parse(data, "query", &result->query);
    return result;
}

DECLARE_PARSER_TO_JSON(ChosenInlineResult) {
    JsonWrapper json;
    if (object) {
        json.put("result_id", object->resultId);
        json.put("from", object->from);
        json.put("location", object->location);
        json.put("inline_message_id", object->inlineMessageId);
        json.put("query", object->query);
    }
    return json;
}

} // namespace TgBot
