#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotCommandScopeAllGroupChats.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotCommandScopeAllGroupChats> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotCommandScopeAllGroupChats>();
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotCommandScopeAllGroupChats> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
