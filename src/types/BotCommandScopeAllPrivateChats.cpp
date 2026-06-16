#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotCommandScopeAllPrivateChats.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotCommandScopeAllPrivateChats> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotCommandScopeAllPrivateChats>();
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotCommandScopeAllPrivateChats> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
