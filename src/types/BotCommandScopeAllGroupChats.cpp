#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotCommandScopeAllGroupChats) {
    auto result = std::make_shared<BotCommandScopeAllGroupChats>();
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommandScopeAllGroupChats) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
