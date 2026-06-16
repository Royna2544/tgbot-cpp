#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ManagedBotUpdated.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ManagedBotUpdated) {
    auto result = std::make_shared<ManagedBotUpdated>();
    result->user = parseRequired<User>(data, "user");
    result->bot = parseRequired<User>(data, "bot");
    return result;
}

DECLARE_PARSER_TO_JSON(ManagedBotUpdated) {
    JsonWrapper json;
    if (object) {
        json.put("user", object->user);
        json.put("bot", object->bot);
    }
    return json;
}

} // namespace TgBot
