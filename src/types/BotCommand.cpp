#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BotCommand.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BotCommand> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BotCommand>();
    parse(data, "command", &result->command);
    parse(data, "description", &result->description);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BotCommand> &object) {
    JsonWrapper json;
    if (object) {
        json.put("command", object->command);
        json.put("description", object->description);
    }
    return json;
}

} // namespace TgBot
