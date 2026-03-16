#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BotCommandScopeChatMember) {
    auto result = std::make_shared<BotCommandScopeChatMember>();
    parse(data, "type", &result->type);
    parse(data, "chat_id", &result->chatId);
    parse(data, "user_id", &result->userId);
    return result;
}

DECLARE_PARSER_TO_JSON(BotCommandScopeChatMember) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("chat_id", object->chatId);
        json.put("user_id", object->userId);
    }
    return json;
}

} // namespace TgBot
