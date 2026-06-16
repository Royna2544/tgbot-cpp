#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotCommandScopeChatAdministrators.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotCommandScopeChatAdministrators> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotCommandScopeChatAdministrators>();
    parse(data, "type", &result->type);
    parse(data, "chat_id", &result->chatId);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotCommandScopeChatAdministrators> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("chat_id", object->chatId);
    }
    return json;
}

} // namespace TgBot
