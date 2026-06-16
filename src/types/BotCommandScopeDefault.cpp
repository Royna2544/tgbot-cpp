#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotCommandScopeDefault.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotCommandScopeDefault> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotCommandScopeDefault>();
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotCommandScopeDefault> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
