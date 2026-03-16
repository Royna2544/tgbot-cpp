#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Dice) {
    auto result = std::make_shared<Dice>();
    parse(data, "emoji", &result->emoji);
    parse(data, "value", &result->value);
    return result;
}

DECLARE_PARSER_TO_JSON(Dice) {
    JsonWrapper json;
    if (object) {
        json.put("emoji", object->emoji);
        json.put("value", object->value);
    }
    return json;
}

} // namespace TgBot
