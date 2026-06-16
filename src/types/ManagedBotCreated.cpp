#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ManagedBotCreated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ManagedBotCreated> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ManagedBotCreated>();
    result->bot = parseRequired<User>(data, "bot");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ManagedBotCreated> &object) {
    JsonWrapper json;
    if (object) {
        json.put("bot", object->bot);
    }
    return json;
}

} // namespace TgBot
