#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotShortDescription) {
    auto result = std::make_shared<BotShortDescription>();
    parse(data, "short_description", &result->shortDescription);
    return result;
}

DECLARE_PARSER_TO_JSON(BotShortDescription) {
    JsonWrapper json;
    if (object) {
        json.put("short_description", object->shortDescription);
    }
    return json;
}

} // namespace TgBot
