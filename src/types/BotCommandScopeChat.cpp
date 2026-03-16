#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotCommandScopeChat) {
    auto result = std::make_shared<BotCommandScopeChat>();
    parse(data, "type", &result->type);
    parse(data, "chat_id", &result->chatId);
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommandScopeChat) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("chat_id", object->chatId);
    }
    return json;
}

} // namespace TgBot
