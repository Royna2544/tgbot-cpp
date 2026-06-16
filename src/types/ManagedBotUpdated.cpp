#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ManagedBotUpdated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ManagedBotUpdated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ManagedBotUpdated>();
    result->user = parseRequired<User>(data, "user");
    result->bot = parseRequired<User>(data, "bot");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ManagedBotUpdated> &object) {
    JsonWrapper json;
    if (object) {
        json.put("user", object->user);
        json.put("bot", object->bot);
    }
    return json;
}

} // namespace TgBot
