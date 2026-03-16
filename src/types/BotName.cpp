#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotName) {
    auto result = std::make_shared<BotName>();
    parse(data, "name", &result->name);
    return result;
}

DECLARE_PARSER_TO_JSON(BotName) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
