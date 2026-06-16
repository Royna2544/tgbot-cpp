#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotCommandScopeChatMember.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotCommandScopeChatMember> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotCommandScopeChatMember>();
    parse(data, "type", &result->type);
    parse(data, "chat_id", &result->chatId);
    parse(data, "user_id", &result->userId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotCommandScopeChatMember> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("chat_id", object->chatId);
        json.put("user_id", object->userId);
    }
    return json;
}

} // namespace TgBot
